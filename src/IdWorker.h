#ifndef CORE_ID_WORKER_H_
#define CORE_ID_WORKER_H_

#include <mutex>
#include <atomic>
#include <chrono>
#include <exception>
#include <sstream>
#include "Noncopyable.h"
#include "Singleton.h"

// 如果不使用 mutex, 则开启下面这个定义, 但是我发现, 还是开启 mutex 功能, 速度比较快
// #define SNOWFLAKE_ID_WORKER_NO_LOCK

namespace SeasSnowFlake {

    /**
    * @brief 核心
    * 核心功能
    */
    namespace Src {

        /**@项目说明 该项目C++ 实现参考如下，感谢以下作者的付出
         * @brief 分布式id生成类  
         * https://gitee.com/Jiawa-code/SnowflakeIdWorker
         * https://segmentfault.com/a/1190000011282426
         * https://github.com/twitter/snowflake/blob/snowflake-2010/src/main/scala/com/twitter/service/snowflake/IdWorker.scala
         *
         * 64bit id: 0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000 
         *           ||                                                           ||     ||     |  |              | 
         *           |└---------------------------时间戳--------------------------┘└中心-┘└机器-┘  └----序列号----┘ 
         *           |
         *         不用
         * SnowFlake的优点: 整体上按照时间自增排序, 并且整个分布式系统内不会产生ID碰撞(由数据中心ID和机器ID作区分), 并且效率较高, 经测试, SnowFlake每秒能够产生26万ID左右.
         */
        class SnowflakeIdWorker : private Noncopyable {

            // 实现单例
            friend class Singleton<SnowflakeIdWorker>;

        public:
            typedef unsigned int UInt;
            typedef unsigned long long int UInt64;

#ifdef SNOWFLAKE_ID_WORKER_NO_LOCK
            typedef std::atomic<UInt> AtomicUInt;
            typedef std::atomic<UInt64> AtomicUInt64;
#else
            typedef UInt AtomicUInt;
            typedef UInt64 AtomicUInt64;
#endif
            int meta[3];
            void setWorkerId(int workerId) {
                this->workerId = workerId;
            }

            void setDatacenterId(int datacenterId) {
                this->datacenterId = datacenterId;
            }
            
            UInt64 stringToUINT64(const std::string s)
            {
                std::stringstream a;
                a << s;
                uint64_t ret = 0;
                a >> ret;
                return ret;
            }

        
            UInt64  degenerate(uint64_t id) {
                uint64_t interval=id >> timestampLeftShift;
                uint dataCenterId=id >> datacenterIdShift;
                uint workerId=id >> workerIdShift;
                uint sequence=workerId << sequenceBits ^ id;
                workerId=dataCenterId << workerIdBits ^ workerId;
                dataCenterId=interval << datacenterIdBits ^ dataCenterId;
                uint64_t timestamp= interval+ (beginTimestamp );
                std::cout <<  "interval is :" << interval  << "    ====";
                std::cout <<  "sequence is :" << sequence  << "   ok ====";
                std::cout <<  "dataCenterId is :" << dataCenterId  << "   ok ====";
                std::cout <<  "workerId is :" << workerId << "    ok====";
                std::cout <<  "beginTime is :" << beginTimestamp << "    ok====";
                std::cout <<  "timestamp is :" << timestamp << "    ====";
                return 0;  
            }

//             AtomicUInt64 getCurrentTimestamp(){
// #ifndef SNOWFLAKE_ID_WORKER_NO_LOCK
//                 std::unique_lock<std::mutex> lock{ mutex };
//                 AtomicUInt64 currentTimestamp{ 0 };
// #else
//                 static AtomicUInt64 currentTimestamp{ 0 };
// #endif
//                 currentTimestamp = getCurrentMillsecondByLocalSystem();     
//                 return currentTimestamp;
//             }

            uint64_t  degenerateTimestamp(uint64_t id) {
                uint64_t interval=id >> timestampLeftShift;
                uint64_t timestamp= interval+ (beginTimestamp );
                return timestamp;
            }

            uint64_t  degenerateBeginTimestamp(uint64_t id) {
                 return beginTimestamp;
            }


            uint  degenerateDataCenterId(uint64_t id) {
                uint64_t interval=id >> timestampLeftShift;
                uint dataCenterId=id >> datacenterIdShift;
                dataCenterId=interval << datacenterIdBits ^ dataCenterId;
                return dataCenterId;
            }

            uint  degenerateWorkerId(uint64_t id) {
                int dataCenterId=id >> datacenterIdShift;
                uint workerId=id >> workerIdShift;
                workerId=dataCenterId << workerIdBits ^ workerId;
                return workerId;
            }

            uint64_t  degenerateInterval(uint64_t id) {
                 uint64_t interval=id >> timestampLeftShift;
                return interval;
            }

            uint  degenerateSequence(uint64_t id) {
                uint workerId=id >> workerIdShift;
                uint sequence=workerId << sequenceBits ^ id;
                return sequence;
            }

            
            

            /**
             * 获得下一个ID (该方法是线程安全的)
             *
             * @return SnowflakeId
             */
            UInt64 generate() {
#ifndef SNOWFLAKE_ID_WORKER_NO_LOCK
                std::unique_lock<std::mutex> lock{ mutex };
                AtomicUInt64 timestamp{ 0 };
#else
                static AtomicUInt64 timestamp{ 0 };
#endif
            // std::cout <<  "lastTimestamp is :" << lastTimestamp << "    ok====";
                timestamp = getCurrentMillsecondByLocalSystem();
               // timestamp=getCurrentMillsecondByHighResolutionClock();
                // 如果当前时间小于上一次ID生成的时间戳，说明系统时钟回退过这个时候应当抛出异常
                if (timestamp < lastTimestamp) {
                    std::ostringstream s;
                    s << "clock moved backwards.  Refusing to generate id for " << lastTimestamp - timestamp << " milliseconds";
                    throw std::exception(std::runtime_error(s.str()));
                }

                if (lastTimestamp == timestamp) {
                    // 如果是同一时间生成的，则进行毫秒内序列
                    sequence = (sequence + 1) & sequenceMask;
                    if (0 == sequence) {
                        // 毫秒内序列溢出, 阻塞到下一个毫秒,获得新的时间戳
                        timestamp = tilNextMillis(lastTimestamp);
                    }
                } else {
                    sequence = 0;
                }

#ifndef SNOWFLAKE_ID_WORKER_NO_LOCK
                lastTimestamp = timestamp;
#else
                lastTimestamp = timestamp.load();
#endif

                // 移位并通过或运算拼到一起组成64位的ID
                return ((timestamp - beginTimestamp) << timestampLeftShift)
                | (datacenterId << datacenterIdShift)
                | (workerId << workerIdShift)
                | sequence;
            }

        protected:
            SnowflakeIdWorker() : workerId(0), datacenterId(0), sequence(0), lastTimestamp(0) { }

            /**
             * 返回以毫秒为单位的当前时间
             *
             * @return 当前时间(毫秒)
             */
            UInt64 timeGen() const {

                uint64_t currentMillsecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                return currentMillsecond;
              //  auto t = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
              //  return t.time_since_epoch().count();
            }

             UInt64 getCurrentMillsecondByLocalSystem() const {

                uint64_t currentMillsecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                return currentMillsecond;
              //  auto t = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
              //  return t.time_since_epoch().count();
            }

             UInt64 getCurrentMillsecondByHighResolutionClock() const {

              
                auto t = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
                return t.time_since_epoch().count();
            }

             UInt64 getCurrentMillsecondByRedis() const {

                uint64_t currentMillsecond = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                return currentMillsecond;
            }

            /**
             * 阻塞到下一个毫秒，直到获得新的时间戳
             *
             * @param lastTimestamp 上次生成ID的时间截
             * @return 当前时间戳
             */
            UInt64 tilNextMillis(UInt64 lastTimestamp) const {
                UInt64 timestamp = timeGen();
                while (timestamp <= lastTimestamp) {
                    timestamp = timeGen();
                }
                return timestamp;
            }

        private:

#ifndef SNOWFLAKE_ID_WORKER_NO_LOCK
            std::mutex mutex;
#endif

            /**
             * 默认开始时间截 (2019-06-20 00:00:00)
             * 可根据配置修改
             */
            const UInt64 beginTimestamp = 1560960000000;

            /**
             * 机器id所占的位数
             */
            const UInt workerIdBits = 5;

            /**
             * 数据中心id所占的位数
             */
            const UInt datacenterIdBits = 5;

            /**
             * 序列所占的位数
             */
            const UInt sequenceBits = 12;

            /**
             * 机器ID向左移12位
             */
            const UInt workerIdShift = sequenceBits;

            /**
             * 数据标识id向左移17位
             */
            const UInt datacenterIdShift = workerIdShift + workerIdBits;

            /**
             * 时间截向左移22位
             */
            const UInt timestampLeftShift = datacenterIdShift + datacenterIdBits;

            /**
             * 支持的最大机器id，结果是31
             */
            const UInt maxWorkerId = -1 ^ (-1 << workerIdBits);

            /**
             * 支持的最大数据中心id，结果是31
             */
            const UInt maxDatacenterId = -1 ^ (-1 << datacenterIdBits);

            /**
             * 生成序列的掩码，这里为4095
             */
            const UInt sequenceMask = -1 ^ (-1 << sequenceBits);

            /**
             * 工作机器id(0~31)
             */
            UInt workerId;

            /**
             * 数据中心id(0~31)
             */
            UInt datacenterId;

            /**
             * 毫秒内序列(0~4095)
             */
            AtomicUInt sequence{ 0 };

            /**
             * 上次生成ID的时间截
             */
            AtomicUInt64 lastTimestamp{ 0 };

        };

        typedef SnowflakeIdWorker IdWorker;
    }
}

#endif // CORE_ID_WORKER_H_
