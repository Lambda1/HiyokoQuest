#ifndef __FPS_CONTROLLER_HPP__
#define __FPS_CONTROLLER_HPP__

#include <chrono>

namespace FPS_CNTR
{
	class FPS
	{
		const int ONE_SECONDS = 1000; /* 1000(msec) */
		const double FPS_60_CNT = 1000.0/60.0; /* 60FPS(16.66...msec) */
		const double FPS_30_CNT = 1000.0/30.0; /* 30FPS(33.33...msec) */
		
		int counter; /* FPS計測カウンタ */
		std::chrono::system_clock::time_point start, end; /* 経過ミリ秒 */
		std::chrono::system_clock::time_point checker_start, checker_end; /* FPSチェック用 */

		public:
			FPS() :
				counter(0),
				start(std::chrono::system_clock::now()), end(std::chrono::system_clock::now()),
				checker_start(std::chrono::system_clock::now()), checker_end(std::chrono::system_clock::now())
			{
			}
			~FPS()
			{
			}
			/* 60fps処理 */
			inline bool Timer_60fps()
			{
				end = std::chrono::system_clock::now(); /* 経過秒 */
				if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > FPS_60_CNT)
				{
					counter++;
					start = std::chrono::system_clock::now(); /* 基準秒の更新 */
					return true;
				}
				return false;
			}
			/* 30fps処理 */
			inline bool Timer_30fps()
			{
				end = std::chrono::system_clock::now(); /* 経過秒 */
				if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > FPS_30_CNT)
				{
					counter++;
					start = std::chrono::system_clock::now(); /* 基準秒の更新 */
					return true;
				}
				return false;
			}
			/* 任意のfps処理 */
			inline bool Timer_Nfps(const double &n_fps)
			{
				end = std::chrono::system_clock::now(); /* 経過秒 */
				if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > n_fps)
				{
					counter++;
					start = std::chrono::system_clock::now(); /* 基準秒の更新 */
					return true;
				}
				return false;
			}
			/* Debug用 */
			/* FPSチェック */
			inline void FPS_Checker()
			{
				checker_end = std::chrono::system_clock::now();
				if (std::chrono::duration_cast<std::chrono::milliseconds>(checker_end - checker_start).count() > ONE_SECONDS)
				{
					std::cout << counter << std::endl;
					counter = 0;
					checker_start = std::chrono::system_clock::now();
				}
			}
	};
};

#endif
