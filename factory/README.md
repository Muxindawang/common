`工厂模式的本质就是对获取对象过程的抽象`

## 简单工厂
定义一个创建对象的接口，子类自己决定实例化哪一个工厂类

*结构：*
- 抽象产品类：定义了要创建的产品对象的接口
- 具体产品类：具有统一父类的具体类型的产品
- 工厂类：负责创建产品对象。

*缺陷：*

扩展性差，违背了开闭原则，新增产品需要修改产品工厂类。

## 工厂方法

在简单工厂的基础上，为每一个产品类增加一个对应的工厂子类，这些工厂子类实现同一个抽象工厂的接口，这样创建不同产品时，只需要实现不同的工厂子类即可，新增产品，也不用修改产品工厂。

*结构：*
- 抽象工厂类：声明了工厂方法的接口
- 具体工厂类：实现工厂方法的接口，负责创建产品对象
- 抽象产品类：定义工厂方法所创建的产品对象的接口
- 具体产品类：具有统一父类的具体类型的产品

*缺陷：*

如果业务涉及的子类越来越多，难道每一个子类都要对应一个工厂类吗？这样会使得系统中类的个数成倍增加，增加了代码的复杂度。

## 抽象工厂
进一步缩减工厂子类的数量，不必给每一个产品分配一个工厂类，而是进行分组，每组不同的产品由同一个工厂类的不同方法创建。
HP不仅生产键盘，还生产显示器，显示器和键盘是同一个HP工程中不同的方法实现。

*结构：*
- 抽象工厂类：声明了创建抽象产品对象的操作接口。
- 具体工厂类：实现了抽象工厂的接口，负责创建产品对象。
- 抽象产品类：定义一类产品对象的接口。
- 具体产品类：定义一个将被相应具体工厂创建的产品对象。

## 模板工厂-注册机制