#include <relacy/relacy_std.hpp>

// A very buggy single producer/single consumer triple buffer
// instrumented with relacy.
template<typename T>
class Trio {
public:
    T& Read() {
        bool isDirty = dirty_($).load(rl::mo_relaxed);
        if (isDirty) {
			front_buffer_ = middle_buffer_($).exchange(front_buffer_, rl::mo_acq_rel);
			dirty_($).store(false, rl::mo_relaxed);
        }

        return *front_buffer_;
    }

    T& Write() {
        return *back_buffer_;
    }

    void Commit() {
        back_buffer_ = middle_buffer_($).exchange(back_buffer_, rl::mo_acq_rel);
        dirty_($).store(true, rl::mo_relaxed);
    }
private:
    static constexpr size_t kNoSharing = 64;
	
	// avoid false-sharing, align buffers to cache-line size
    struct alignas(kNoSharing) Buffer {
        T data{0};
    };

    Buffer buffers_[3];

    rl::atomic<bool> dirty_{false};
    rl::atomic<T*> middle_buffer_{&buffers_[1].data};

    alignas(kNoSharing)
    T* front_buffer_{&buffers_[0].data};
    alignas(kNoSharing)
    T* back_buffer_{&buffers_[2].data};
};
