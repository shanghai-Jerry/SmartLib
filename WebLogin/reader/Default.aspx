<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="reader_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
      <a href="/Default.aspx">主界面</a> > <a href="Default.aspx"> 读者区</a>
      
    </div>
    <hr />
    <asp:GridView ID="GridView1" runat="server" AutoGenerateColumns="False" AllowPaging="True"
        AllowSorting="True" ShowFooter="True" ShowHeaderWhenEmpty="True" Width="100%"
        PageSize="20" >
        <Columns>
            <asp:BoundField DataField="rid" HeaderText="读者编号" />
            <asp:BoundField DataField="name" HeaderText="姓名" />
            <asp:BoundField DataField="sex" HeaderText="性别" />
            <asp:BoundField DataField="birth" HeaderText="出生年月" />
            <asp:BoundField DataField="major" HeaderText="专业 " />
            <asp:BoundField DataField="rtype" HeaderText="读者类型" />
            <asp:BoundField DataField="status" HeaderText="身份" />
           
            
        </Columns>
        
    </asp:GridView>
    <table style="width: 70%">
        <tr style="width: 80%">
            <td>
                <div align="left" style="font-size: 9pt">
                    <asp:LinkButton ID="btnFirst" OnClick="PagerButtonClick" runat="server" Font-Bold="True"
                        ForeColor="HotTrack" CommandArgument="first" Visible="false">首页||</asp:LinkButton>
                    <asp:LinkButton ID="LinkButton2" OnClick="PagerButtonClick" runat="server" Font-Bold="True"
                        ForeColor="HotTrack" CommandArgument="prev" Visible="false">前页||</asp:LinkButton>
                    <asp:LinkButton ID="LinkButton1" OnClick="PagerButtonClick" runat="server" Font-Bold="True"
                        ForeColor="HotTrack" CommandArgument="next" Visible="false">下页||</asp:LinkButton>
                    <asp:LinkButton ID="LinkButton3" OnClick="PagerButtonClick" runat="server" Font-Bold="True"
                        ForeColor="HotTrack" CommandArgument="last" Visible="false">尾页</asp:LinkButton></div>
            </td>
            <td style="font-size: 9pt" class="style2">
                <div align="right">
                    <asp:Label ID="lblCurrentIndex" runat="server" Font-Bold="True" ForeColor="HotTrack"
                        Visible="false"></asp:Label>
                    <asp:Label ID="lblPageCount" runat="server" Font-Bold="True" ForeColor="HotTrack"
                        Visible="false"></asp:Label></div>
            </td>
        </tr>
        </table>
    </form>
</body>
</html>
