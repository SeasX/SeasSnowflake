#ifndef Src_TIMER_H_
#define Src_TIMER_H_

#include <chrono>

namespace SeasSnowFlake {

    /**
    * @brief 核心
    * 核心功能
    */
    namespace Src {

        /**
        * @brief 计时器类
        * 计时
        */
        template <typename CLOCK = std::chrono::high_resolution_clock>
        class Timer {
        public:

            typedef CLOCK clock;
            typedef typename CLOCK::rep rep;
            typedef typename CLOCK::duration duration;
            typedef typename CLOCK::time_point time_point;

            Timer() : m_start(CLOCK::now()) {
            }

            /**
            * @brief 重置计时器
            *
            * @return 开始的时间点
            */
            time_point reset() {
                m_start = CLOCK::now();
                return m_start;
            }

            /**
            * @brief 得到流逝的时间
            * @param UNIT 返回时间的类型
            *
            * @return 返回流逝的时间
            */
            template <typename UNIT = std::chrono::milliseconds>
            typename UNIT::duration::rep elapsed() const {
                return std::chrono::duration_cast<UNIT>(CLOCK::now() - m_start).count();
            }

        private:
            time_point m_start;
        };
    }
}

#endif // CORE_TIMER_H_