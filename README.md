# EventlogAnalyzer
Win10系统日志事件分析

通过微软提供的接口OpenEventLog()、ReadEventLog()和CloseEventLog()，读取原始事件集，保存在线性表中。

事件的属性包括事件ID、级别、类别、时间、计算机名以及用户信息等。

参考微软开发文档：https://docs.microsoft.com/zh-cn/windows/desktop/EventLog/event-logging-operations

由于（本人所使用机器的）Windows系统日志事件属性中，几乎所有事件的Correlation（关联）一栏都为空，所以无法获得原始的事件关联关系。这里采用统计的方法统计关联规则。即当事件A与在其发生后的Delta_Time时间内发生的事件B、C、D……建立A->B、A->C、A->D、……的关联规则。统计这些规则的数目。

遍历关联规则，通过置信度公式
