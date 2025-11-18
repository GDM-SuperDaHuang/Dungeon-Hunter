# Dungeon-Hunter
ue5
一、行为树：
节点有3个状态
    Running（正在执行中）
    Succeeded（成功）
    Failed（失败）


执行规则：
一个节点运行完毕后，行为树会逐层往上返回结果，并让上层节点决定接下来的行为。
如：一个字节汇报给Sequence父节点，父节点决定继续运行其他的子节点


二、父节点类型：
Selector 、 Sequence 、Parallel

Selector：单任务类型，分支上的所有节点，只要有一个执行成功则，停止接下来的运行。
Sequence：单任务类型，分支上的所有节点，只要有一个执行失败则，停止接下来的运行。
Parallel：并行任务，主任务（Main Task） 阻塞整个节点直到它完成；背景树（Background Branch） 里的所有子任务 与主任务并行执行；节点最终成功/失败 只看主任务结果（背景树成败不影响）。

注意！成功和失败的定义不是装饰器的条件，而是 EBTNodeResult 返回的结果，即为，成功→FinishExecute(true) ；失败→FinishExecute(false)

eg：
Sequence
 ├── Task A
 ├── Task B
 └── Task C
 如果：
    A:Success
    B:Failure
    C:不再执行
    整棵Sequence返回执行完毕结果


Selector
 ├── Task A
 ├── Task B
 └── Task C

  如果：
    A:Failure
    B:Success
    C:不再执行
    整棵Selector返回执行完毕结果


Parallel
├─ 主任务: Task A （MoveTo，需 5 秒）
└─ 背景分支: Sequence
    ├─ Task B （每 0.5s 打印一次）
    └─ Task C （每 1s 播放音效）
    直到Task A完成，整棵Parallel返回执行完毕结果



三、装饰器：
某些装饰器可以设置为动态评估（Observer Aborts）。
这控制了运行过程中装饰器条件是否会被重新检查

| 检查时机                      | 说明             |
| ------------------------      | -------------- |
| **节点执行前**                | 决定节点是否能进入。     |
| **运行中（Observer Aborts）** | 决定是否中断自己或兄弟节点。 |
| **Blackboard变化时**          | 自动触发重新评估条件。    |

    
3.1、通知观察者：
    是否进行重新评估？
    1,OnResultChange 结果改变时：条件翻转时候触发。
    2,OnValueChange 值改变时：SetValue ，内存写一下就触发，无论是否黑板键值是否和旧的重复。
        eg:黑板键float类型 ，条件：Health > 50.0，上一帧 Evaluate 结果：false（因为 Health=40）
        | 时刻 | Health 新值 | 数值变了吗？| 返回值变了吗？ | OnResultChange 触发？ | OnValueChange 触发?|
        | --- | ----------- | ------    | ------------   | ------------------    |-------------------
        | t=1 | 40 → **40** | ❌ 相同   | ❌ 仍为 false  | ❌ **不触发**         |✅ **触发**（翻转）|
        | t=2 | 40 → **60** | ✅ 不同   | ✅ false→true  | ✅ **触发**（翻转）   |✅ **触发**（翻转）|
        | t=3 | 60 → **80** | ✅ 不同   | ❌ 仍为 true   | ❌ **不触发**（无翻转）|✅ **触发**（翻转）|

3.2、观察者中止：
经过评估后，如何行为？
| 模式                | 谁触发中断？                     | 触发时的行为                  |
| ------------------ | -------------------------------- | ---------------------------- |
| **None**           | 永不触发                         | 不动态重新评估。                |
| **Self**           | 自己条件变为 false               | 中断自己（当前分支）。             |
| **Lower Priority** | 自己条件变为 false→true          | 中断该装饰器右边/优先级更低的分支。 |
| **Both**           | 条件翻转,变为 false/true         | 同时触发 Self 和 Lower Priority ，综合两种行为。|

    1，None :只在节点开始前检查一次，不会动态中断
    2，Self :当前节点运行期间，如果装饰器条件变为 false，会中断该节点（自己）
            ·触发条件：装饰器条件从真→假，时触发
    3，Lower Priority：
            ·作用：只打断“同一父节点下的右侧兄弟
            ·触发条件：装饰器条件从假→真，时触发

    4，Both：
        ·触发条件：真→假或者假→真时触发(双向)


![alt text](image.png)

检查范围和时机
1，检查的位置和时机 和正在运行的节点有关
2，检查的路径和激活的路径相关，在这个Root到running Node路径上所有节点，以及这些节点的儿子子节点的所有装饰器。
3，检查的时机，分两种，Blackboard Key 值变化（Observer 机制）和每帧检查（部分装饰器类型或自定义条件）
    | 类型                        | 触发条件                               | 作用对象          |
    | ----------------------      | -------------------------------       | ---------------- |
    | **事件驱动（Event-driven）** | Blackboard Key 值变化（Observer 机制） | 所有依赖该 Key 的活跃装饰器 |
    | **周期检查（Tick-driven）**  | 每帧检查（部分装饰器类型或自定义条件）   | 当前激活路径上的装饰器      |




eg：宠物AI行为
                                                      Root
                                                       |     
                                             ———— Selector_Root ————————
                                            |                           |
                Sequence_Attack(D:LP,IsEnemyVisible)                 Sequence_Patrol
                                    |                                       |
                        ———————————————————————                      ————————————————————————————————
                       |                      |                     |                                |
AttackEnemy(D:self,IsInAttackRange)         Taunt         MoveToNextPoint(D:self,HasPatrolPath)     Wait

阶段1：
    初始状态（敌人不可见），进行巡逻行为
    IsEnemyVisible = false
    HasPatrolPath = true
    激活路径：Root → Selector_Root → Sequence_Patrol → MoveToNextPoint
此时会检查：激活路径上节点以及儿子节点的所有装饰器，即，MoveToNextPoint，Sequence_Attack。

阶段 2：
    敌人出现
    IsEnemyVisible = false → true
    因为D:LP，则打断之前的MoveToNextPoint。
    IsInAttackRange = false
    激活路径：Root → Selector_Root → Sequence_Attack → Taunt

阶段 3：
    攻击阶段中（敌人在攻击范围内）
    IsInAttackRange=false → true
    等待Taunt运行结束，结束后，→汇报Sequence_Attack → 汇报Selector_Root→ 汇报Root，root重新评估
    IsEnemyVisible = true
    激活路径：Root → Selector_Root → Sequence_Attack → AttackEnemy

阶段 4：
    敌人消失
    IsEnemyVisible = true → false
    中断AttackEnemy，如果放回值是FinishExecute(true)，则进行Taunt，否则父节点重新层层向上汇报，重新评估



EQS:

一、Generator
1）Points: Circle（圆形）
    选项包含：
    半径（Radius）
    间隔角度（Angle Step）
    点数量限制
    方向（Clockwise/Counter）
    在导航网格上贴合（Project to Navmesh）

2）Points: Grid（正方形格子）
    Cell Size
    Grid Size
    2D / 3D

3）Points: Donut（环形）
    内外半径
    采样点数量

4）Points: Pathing Grid
    基于导航网格生成点（对 AI 最友好）

5）Actors of Class
    指定类（例如找到场上所有 NPC、所有敌人）
    距离过滤
    是否 Project 到 navmesh

6）Single Item
    例如 Target Actor 的位置
