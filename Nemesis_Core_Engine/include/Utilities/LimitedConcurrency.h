#include <memory>
#include <condition_variable>

namespace nemesis
{
	struct LimitedConcurrency
    {
    private:
        unsigned int MaxThreads;
        unsigned int CurrentThreads;

        std::mutex Mutex;
        std::condition_variable CondVar;

    public:
        LimitedConcurrency(unsigned int max_threads) noexcept;

        void Lock();
        void Unlock();
    };
}
