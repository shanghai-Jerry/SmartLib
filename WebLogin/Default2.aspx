<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Default2.aspx.cs" Inherits="Default2" Debug="true" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>登陆</title>
    <style type="text/css">
        #input_rid {
            height: 13px;
            width: 131px;
        }
        #bid {
            height: 13px;
            width: 129px;
        }
        .auto-style1 {
            text-align: justify;
            position: relative;
        }
    </style>
</head>
<body>
    
    <form id="form1" runat="server">
    <div class="auto-style1" style="text-align: center;">
       <br />
      <label  id="rid" >读者编号：</label>
      <input  type="text"  id="input_rid" title="请填写编号" runat="server" style="width:300px; height:34px;" />
         <br />
      <label  title="密码">
        <br />
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        密&nbsp;&nbsp;&nbsp; 码：&nbsp; </label>
       <input type="password" id="passwd"  maxlength="18" size="24" style="width:300px; height:34px; " value="" name="psw" runat="server" />
        <asp:button id="Button1" runat="server"  Text="登陆"   OnClick="Button1_Click"/>
      &nbsp;<br />
        <br />
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      <input  type="text"  id="bid" value="readOnly"  runat="server" visible="false"/>
       
     </div>
       
    </form>
</body>
</html>
