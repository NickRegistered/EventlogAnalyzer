# EventlogAnalyzer
Win10系统日志事件分析

读取Windows系统日志事件信息及相关关系，并用图结构表示。提供事件信息的插入、删除和检索功能。并通过相关关系对事件进行简单的预测和溯源。

通过微软提供的接口OpenEventLog()、ReadEventLog()和CloseEventLog()，读取原始事件集，保存在线性表中。

事件的属性包括事件ID、级别、类别、时间、计算机名以及用户信息等。

参考微软开发文档：

有关系统日志的API介绍：https://docs.microsoft.com/zh-cn/windows/desktop/EventLog/event-logging-operations

有关系统时间结构体的介绍https://docs.microsoft.com/zh-cn/windows/desktop/api/winnt/ns-winnt-_eventlogrecord

由于（本人所使用机器的）Windows系统日志事件属性中，几乎所有事件的Correlation（关联）一栏都为空，所以无法获得原始的事件关联关系。这里采用统计的方法统计关联规则。即当事件A与在其发生后的Delta_Time时间内发生的事件B、C、D……建立A->B、A->C、A->D、……的关联规则。统计这些规则的数目。

首先统计各事件(以ID区分)发生的次数，将满足最小支持度的事件加入到关联图中。

遍历关联规则，通过置信度公式confidence（X−>Y）=σ(X∪Y)/σ(X)，计算出每条关联规则的置信度，并删除低于最低置信度的规则，剩下的关联规则称强关联规则。

将结点（以事件ID区分）和关联规则采用邻接表的方式存储，其结构示意图如下:

![邻接表结构示意图](https://github.com/NickRegistered/EventlogAnalyzer/blob/master/pictures/%E4%BA%8B%E4%BB%B6%E5%9B%BE%E7%BB%93%E6%9E%84%E7%A4%BA%E6%84%8F%E5%9B%BE.png)

结点采用线性表的方式顺序存储，关联规则作为弧，链接到结点下，构成邻接表。

