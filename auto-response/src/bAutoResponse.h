
int nCheckCount;//用于计算验证用户或密码(包括开始的1#和2#)的次数，最多三次
int nFlag;//0表示其他，1表示正在用户名验证，2表示正在密码验证 
int bAutoResponse(int ModeID, CString str);
int OtherCmdDeal(CString str);

