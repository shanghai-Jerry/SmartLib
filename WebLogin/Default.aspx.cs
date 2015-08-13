using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using MySql.Data.MySqlClient;

public partial class _Default : System.Web.UI.Page 
{
    protected void Page_Load(object sender, EventArgs e)
    {
        this.rid.Value = Request.QueryString["rid"];
        this.status.Value = Request.QueryString["status"];
        this.bid.Value = Request.QueryString["bid"];
        if (!IsPostBack)
        {
            if (this.rid.Value == "")
            {
                this.rid.Value = Session["rid"] as string;
            }
            if (this.bid.Value == "")
            {
                this.bid.Value = Session["bid"] as string;
            }
            if (this.status.Value == "")
            {
                this.status.Value = Session["status"] as string;
            }
        }
        string connstr = "server=localhost;user id=admin;password=123456;database=mydb";
        string sqlQuery;
        if (this.bid.Value.CompareTo("1") ==0)
        {
            sqlQuery = "update admin set login=1 where rid='" + this.rid.Value + "'"; 
        }
        else
        {
            sqlQuery = "update reader set login=1 where rid='" + this.rid.Value + "'"; ;
        }
        using (MySqlConnection conn = new MySqlConnection(connstr))
        {
           // string sqlQuery = "update admin,reader set login=1 where rid='" + this.rid.Value + "'";
            MySqlCommand comm = new MySqlCommand(sqlQuery, conn);
            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                throw new Exception("数据库连接失败!", ex);
            }
            comm.ExecuteNonQuery();

        }
    }

    protected void Button1_Click(object sender, EventArgs e)
    {
        string sqlQuery;
        string connstr = "server=localhost;user id=admin;password=123456;database=mydb";
        if (this.bid.Value.CompareTo("1") == 0)
        {
            sqlQuery = "update admin set login=0 where rid='" + this.rid.Value + "'";
        }
        else
        {
            sqlQuery = "update reader set login=0 where rid='" + this.rid.Value + "'"; 
        }
        
        using (MySqlConnection conn = new MySqlConnection(connstr))
        {
           // string sqlQuery = "update admin,reader set login=1 where rid='" + this.rid.Value + "'";
            MySqlCommand comm = new MySqlCommand(sqlQuery, conn);
            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                throw new Exception("数据库连接失败!", ex);
            }
            comm.ExecuteNonQuery();

        }
        Response.Redirect("Default2.aspx");
    }
}