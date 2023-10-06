# EnTT-skpacman

本项目改编自我之前的一个项目[pacman-SDL2](https://github.com/CN-Shopkeeper/pacman-SDL2)，目的在于熟悉、使用 ECS 系统，引入了[EnTT](https://github.com/skypjack/entt)作为游戏底层。

参考了[EnTT-Pacman](https://github.com/indianakernick/EnTT-Pacman)。

回顾、移植之前的这个游戏，感慨良多。完善了很多写法，并且在尽可能保持原有游戏机制的前提下增加了少许新的特性。

## 与 Pacman-SDL2 的区别

- 移除了排行榜、操作提示等等用户友好的功能，专注于 ECS 系统的继承。

- 游戏窗口大小根据屏幕大小确定

- 为 Ghost 增加了 Eaten 状态，直观体现是被吃完后不会立刻闪现在 GhostHouse，而是有一个跑回去的过程

- Ghost 现在不可以随意进出 GhostHouse 了

- Pacman 死亡后也不会传送回复活点，而是原地进入无敌状态
