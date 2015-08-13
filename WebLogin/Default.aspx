<%@ Page Language="C#" AutoEventWireup="true"  CodeFile="Default.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>确认操作</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
      
      <label  title="读者编号">读者编号：</label>
      <input type="text" id="rid" runat="server" />
       <label  id="状态" >
        状&nbsp; 态 ：</label>
      <input  type="text"  id="status" runat="server"/>
        标&nbsp; 记<label  id="id" > ：</label>
      <input  type="text"  id="bid" value="readOnly" readonly="readonly" runat="server"/>
        <asp:Button ID="Button1" runat="server" Height="21px" OnClick="Button1_Click" Text="注销用户" />
    </div>
    <div>
        <a href="library/Default.aspx">图书管理</a>
        <a href="reader/Default.aspx">读者管理</a>
        <a href="borrow/Default.aspx?rid=">借阅管理</a>
        <a href="system/Default.aspx">系统管理</a>
    </div>
     
    
    </form>
</body>
</html>
