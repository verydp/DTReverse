//手动新增节，不影响原始程序的使用

//思路(最好还是借助PE工具，对于修改的节信息比较直观)
// 涉及修改的原属性有sizeofimage、numberofsection
// 涉及新增的只有节数据和节表
// 新增的节表参照工具上面的节表信息会更容易直观的修改
//1、修改节的数量
//2、新增节表
//3、添加节。修改节表的三个属性(pointertorawdata、sizeofrawdata、virtualaddress)
//4、修改sizeofimage