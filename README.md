# 2024华为软件精英挑战赛
“唐可可后援会”队在2024华为软件精英挑战赛初赛的相关代码与测试程序

# Huawei CodeCraft2024
Repository of Team "Fanclub for Tang Keke" in Huawei CodeCraft 2024

-----------
## 赛后更新: 
第一次参赛，初赛主要使用了基础的BFS作为寻路算法，获奖还是比较幸运的。

程序中添加了一些防撞优化（handle_crash()函数）、价值判断优化（寻路过程中以$2*distance - good.val$ 作为判断依据）和船舶调度优化，并在正式赛中针对地图做了特定优化，在初赛中排名第九。

`test.py`是专门写的运行不同种子、地图的自动化程序，可作为测试平均成绩使用。

## Update:
This is our first time participating in CodeCraft. I mainly used basic BFS as the pathfinding algorithm in the preliminary round. It was quite lucky to win the prize.

Some anti-collision optimization (handle_crash() function), value judgment optimization ($2*distance - good.val$ is used as the basis for judgment during the pathfinding process) and ship scheduling optimization were added to the program, and were made for the map in the official competition. Specific optimization, ranked ninth in the preliminary round.

`test.py` is an automated program specially written to run different seeds and maps, and can be used as testing average score.
