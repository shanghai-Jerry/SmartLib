using System.Windows;
using MySql.Data.MySqlClient;
using System;
using System.Collections;
using System.Configuration;
using System.Data;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts; 
public partial class Default2 : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        //扫描二维码登陆，其他登陆方式测试需要注释这三行
       this.input_rid.Value = Request.QueryString["rid"];
       this.bid.Value = Request.QueryString["bid"];
       
   
    }
    public DataTable GetINFO(string connectionString,string SQL)
    {
        DataTable dt = new DataTable();
        //string connStr = "server=localhost;user id=admin;password=123456;database=wms_inventory";

        using (MySqlConnection conn = new MySqlConnection(connectionString))
        {
            try
            {
                MySqlCommand command0 = new MySqlCommand(SQL);
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

    protected void Button1_Click(object sender, EventArgs e)
    {
        string sqlQuery;
        string connstr = "server=localhost;user id=admin;password=123456;database=mydb";
        if (this.bid.Value.CompareTo("1") == 0)
        {
            sqlQuery = "select pwd from admin where rid='" + this.input_rid.Value + "'";
        }
        else
        {
            sqlQuery = "select pwd from reader where rid='" + this.input_rid.Value + "'";
        }
        using (MySqlConnection conn = new MySqlConnection(connstr))
        {
           
           // string sqlQuery = "select pwd from admin where rid='" + this.input_rid.Value + "'";
            MySqlCommand comm = new MySqlCommand(sqlQuery, conn);
            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                throw new Exception("数据库连接失败!", ex);
            }
            MySqlDataReader dr = comm.ExecuteReader();

            if (dr.Read())
            {
                string dpwd = dr[0].ToString();
                if (dpwd.CompareTo(this.passwd.Value) == 0)
                {
                    Response.Write("<script>alert('Log success');</Script>");
                    Session["rid"] = this.input_rid.Value;
                    Session["bid"] = this.bid.Value;
                    Session["status"] = "确认成功";
                    Response.Redirect("Default.aspx?rid=" + this.input_rid.Value + "&status=确认成功&bid="+this.bid.Value);
                    

                }
            }
            else
            {
                Response.Write("<script>alert('Log Failed');</Script>");
             }
            DataTable DT = GetINFO(connstr,sqlQuery);
           
           
        }
        
       
    }
}
