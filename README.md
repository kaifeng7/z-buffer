# z-buffer
scanline

## 数据结构

### 边表

1）没有使用delta_y，而是使用y_min代表最后一条扫描线，这样判断起来更为方便。
2）先计算delta_x，每次扫描只需要将边x+=delta_x；更快捷。
3）先计算delta_Zx,delta_Zy，每次扫描只需要将边z+=delta_x*delta_Zx+delta_Zy；更快捷。
4) 将活化边表和分类边表使用同一中数据结构，这样在将分类边表加入活化边表时，不需要重新new一个节点，也不需要赋值操作，直接修改指针就好。

### 多边形表
1）没有使用delta_y，而是使用y_min代表最后一条扫描线，这样判断起来更为方便
2）将活化多边形表和分类多边形表使用同一中数据结构，这样在将分类多边形表加入活化多边形表时，不需要重新new一个节点，也不需要赋值操作，直接修改指针就好。
3）构建std::vector<Polygon>，即哈希存储，存储的多边形id就是其索引号，这样在扫描活化边属于哪个多边形时，可以直接根据其id号找到，复杂的O（1）。
4）在Polygon中存储多边形的基本属性，而在多边形表中只存储y_min和id号，这样多边形链表就会大大减少存储空间。

### buffer

使用一个全局二位数组buffer，未使用深度缓存，而是在进入一个新的多边形时，将其z值与max_z做比较，如果大于max_z时，则覆盖。然后将max_z所代表的多边形id号存入buffer，在render时使用。


