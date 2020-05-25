## 中间代码IR码的定义：

#### 变量相关：

中端负责解析变量定义并将其转化为唯一的标识。当变量定义时，使用malloc语句声明在**局部**的内存空间。结构体变量对每一个子元素拆分，转化为单独的普通变量维护。数组元素长度可能很长，所以保留数组定义。

| op     | v1   | v2             | v3               | 描述            |
| ------ | ---- | -------------- | ---------------- | --------------- |
| malloc | t1   | size(byte单位) |                  | t1=malloc(size) |
| array  | t1   | t2             | pos(下标从0开始) | t1=t2[pos]      |
| int    | t1   | t2             |                  |                 |
| real   | t1   | t2             |                  |                 |
#### 运算相关：

运算时，运算的数据类型判断在中端进行。运算方式通过运算符区分

| op                                     | v1   | v2   | v3   | 描述                           |
| -------------------------------------- | ---- | ---- | ---- | ------------------------------ |
| posivite, negative, neg_logic, neg_bit | t1   | t2   |      | t1=op t2 （+-!~                |
| add_i, minus_i, mult_i, div_i, mod_i   | t1   | t2   | t3   | t1=t2 op t3  (+-*/%            |
| add_f, minus_f, mult_f, div_f,         | t1   | t2   | t3   | t1=t2 op t3  (+-*/             |
| gt, lt, ge, le                         |      | t2   | t3   | t1=t2 op t3  (>,<,>=,<=        |
| and_logic, or_logic                    | t1   | t2   | t3   | t1=t2 op t3  (&&, \|\|         |
| and_bit, or_bit, xor_bit, sl, sr       | t1   | t2   | t3   | t1=t2 op t3  (&, \|, ^, <<, >> |

#### 分支控制

label控制分支跳转。

函数相关：函数定义时规定函数参数的个数n，函数参数默认为($arg_0,arg_1...arg_{n-1}$)。调用时param用于参数的传递，这里的参数会通过栈结构保存，调用call f n时会取param栈的最上面n个从下到上映射到函数参数（push arg0, push arg1....)，请注意对应关系。此处绑定均为**值**的绑定，引用的参数传递请传递地址。

| op       | v1    | v2    | v3   | 描述                            |
| -------- | ----- | ----- | ---- | ------------------------------- |
| label    | name  |       |      | label: name                     |
| goto     | label |       |      | goto label                      |
| if       | v1    | label |      | if v1 goto label                |
| func     | name  | n     |      | func($arg_0,arg_1...arg_{n-1}$) |
| param    | v1    |       |      | param v1                        |
| call     |       | func  | n    | call func(n params)             |
| return   |       |       |      | return                          |
| call_v   | v1    | func  | n    | v1 = func(n params)             |
| return_v | v1    |       |      | return v1                       |

#### 系统调用

与函数相同调用方法，通过param传参

| op    | 描述 |
| ----- | ---- |
| write |      |
|       |      |

