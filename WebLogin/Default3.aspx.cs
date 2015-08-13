using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class Default3 : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        
        if (Request.UrlReferrer != null) // 
        {
            this.status.Value = "success";

            //ViewState["UrlReferrer"] = Request.UrlReferrer.ToString();
            //Response.Write(Request.UrlReferrer.ToString());
        }
        else
        {
            this.status.Value = "failed";
        }
        
    }
}