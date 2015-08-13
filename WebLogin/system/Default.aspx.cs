using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class system_Default : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        var data = GetDataSource();
        GridView1.DataSource = data;
        GridView1.DataBind();
        if (data != null && data.Rows.Count > 0)
        {
            int pageCount = 0;

            if (data.Rows.Count % GridView1.PageSize > 0)
            {
                pageCount = data.Rows.Count / GridView1.PageSize + 1;
            }
            else
            {
                pageCount = data.Rows.Count / GridView1.PageSize;
            }
            lblCurrentIndex.Text = "第1页";
            lblPageCount.Text = "/共" + pageCount + "页";

            btnFirst.Visible = true;
            btnFirst.Enabled = false;
            LinkButton2.Visible = true;
            LinkButton2.Enabled = false;
            LinkButton1.Visible = true;
            LinkButton3.Visible = true;
            lblCurrentIndex.Visible = true;
            lblPageCount.Visible = true;
        }
    }
    public void PagerButtonClick(Object sender, EventArgs e)
    {
        //获得LinkButton的参数值
        string arg = ((LinkButton)sender).CommandArgument;

        switch (arg)
        {
            case ("next"):
                if (GridView1.PageIndex < (GridView1.PageCount - 1))
                    GridView1.PageIndex++;
                if (GridView1.PageIndex == GridView1.PageCount - 1)
                {
                    btnFirst.Enabled = true;
                    LinkButton2.Enabled = true;
                    LinkButton1.Enabled = false;
                    LinkButton3.Enabled = false;
                }
                else
                {
                    btnFirst.Enabled = true;
                    LinkButton2.Enabled = true;
                    LinkButton1.Enabled = true;
                    LinkButton3.Enabled = true;
                }
                break;
            case ("prev"):
                if (GridView1.PageIndex > 0)
                    GridView1.PageIndex--;
                if (GridView1.PageIndex == 0)
                {
                    btnFirst.Enabled = false;
                    LinkButton2.Enabled = false;
                    LinkButton1.Enabled = true;
                    LinkButton3.Enabled = true;
                }
                break;
            case ("last"):
                GridView1.PageIndex = (GridView1.PageCount - 1);
                btnFirst.Enabled = true;
                LinkButton2.Enabled = true;
                LinkButton1.Enabled = false;
                LinkButton3.Enabled = false;
                break;
            case ("first"):
                GridView1.PageIndex = 0;
                btnFirst.Enabled = false;
                LinkButton2.Enabled = false;
                LinkButton1.Enabled = true;
                LinkButton3.Enabled = true;
                break;
        }

        GridView1.DataSource = GetDataSource();
        GridView1.DataBind();
        ShowPageIndex();
    }
    public void ShowPageIndex()
    {
        //确定页
        lblCurrentIndex.Text = "第" + ((Int32)GridView1.PageIndex + 1) + "页";
        lblPageCount.Text = "/共" + GridView1.PageCount + "页";
    }
    private DataTable GetDataSource()
    {
        DataTable dt = new DataTable();
        string connstr = "server=localhost;user id=admin;password=123456;database=mydb";
        using (MySqlConnection conn = new MySqlConnection(connstr))
        {
            //bookid,isbn,bookname,author,publish,pubtime,price,storenum,introduction
            try{
                StringBuilder selstr = new StringBuilder();
                selstr.AppendLine(@"SELECT * from readertype
                                        WHERE
	                                        1 = 1");
                MySqlCommand command0 = new MySqlCommand(selstr.ToString());
                command0.Connection = conn;
                conn.Open();

                MySqlDataAdapter dataAdapter = new MySqlDataAdapter(command0);
                dataAdapter.Fill(dt);
            }
            catch (Exception ex)
            {
                throw new Exception(ex.Message, ex);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }
        }

        return dt;
    }
}