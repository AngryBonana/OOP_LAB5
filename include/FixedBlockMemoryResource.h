#ifndef FIXED_BLOCK_MEMORY_RESOURCE_H
#define FIXED_BLOCK_MEMORY_RESOURCE_H

#include <memory_resource>
#include <map>
#include <cstddef>
#include <new>
#include <vector>

class FixedBlockMemoryResource : public std::pmr::memory_resource {
public:
    FixedBlockMemoryResource(std::size_t block_size, std::size_t object_size)
        : object_size_(object_size), alignment_(alignof(std::max_align_t)) {
        if (object_size_ == 0) object_size_ = 1;

        object_size_ = (object_size_ + alignment_ - 1) & ~(alignment_ - 1);
        pool_size_ = (block_size / object_size_) * object_size_;
        pool_ = static_cast<char*>(::operator new(pool_size_, std::nothrow));
        if (!pool_) throw std::bad_alloc{};

        for (std::size_t i = 0; i < pool_size_; i += object_size_) {
            free_blocks_.push_back(pool_ + i);
        }
    }

    ~FixedBlockMemoryResource() {
        if (pool_) ::operator delete(pool_);
    }

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        if (bytes > object_size_ || alignment > alignment_) {
            throw std::bad_alloc{};
        }
        if (free_blocks_.empty()) {
            throw std::bad_alloc{};
        }
        void* ptr = free_blocks_.back();
        free_blocks_.pop_back();
        allocations_[ptr] = true; 
        return ptr;
    }

    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        (void)bytes; (void)alignment;
        if (!p) return;
        if (allocations_.erase(p)) {
            free_blocks_.push_back(static_cast<char*>(p));
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

private:
    std::size_t object_size_;
    std::size_t alignment_;
    char* pool_ = nullptr;
    std::size_t pool_size_ = 0;
    std::vector<char*> free_blocks_;
    std::map<void*, bool> allocations_;
};

#endif