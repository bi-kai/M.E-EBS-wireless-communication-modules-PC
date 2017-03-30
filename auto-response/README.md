自动应答已经添加完成！
1、添加函数 int CBeidouDlg::bAutoResponse(int ModeID, CString str)
2、添加函数 int CBeidouDlg::OtherCmdDeal(CString str)

3、其他添加的话，在文件中都以//------- 标注
4、两个全局变量：
	int nCheckCount;//用于计算验证用户或密码(包括开始的1#和2#)的次数，最多三次
	int nFlag;//0表示其他，1表示正在用户名验证，2表示正在密码验证 
5、注：每次操作都是要#号结束，12#为一次操作，12#3#为两次操作