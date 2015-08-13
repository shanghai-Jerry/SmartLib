<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="borrow_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
      <a href="/Default.aspx">主界面</a> > <a href="Default.aspx">借阅区</a>
    </div>
    <hr />
    <asp:GridView ID="GridView1" runat="server" AutoGenerateColumns="False" AllowPaging="True"
        AllowSorting="True" ShowFooter="True" ShowHeaderWhenEmpty="True" Width="100%"
        PageSize="20" >
        <Columns>
            <asp:BoundField DataField="rid" HeaderText="读者编号" />
            <asp:BoundField DataField="bookid" HeaderText="图书编号" />
        
            <asp:BoundField DataField="bookname" HeaderText="图书名称" />
           
            <asp:BoundField DataField="borrowtime" HeaderText="借阅日期 " />
            <asp:BoundField DataField="publish" HeaderText="出版社" />
            <asp:BoundField DataField="returntime" HeaderText="归还日期" />
            <asp:BoundField DataField="times" HeaderText="续借次数" />
            <asp:BoundField DataField="dealwith" HeaderText="处理类型" />
            <asp:BoundField DataField="status" HeaderText="处理状态" />
            
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
