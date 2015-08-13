using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Text;
using ThoughtWorks;
using ThoughtWorks.QRCode;
using ThoughtWorks.QRCode.Codec;
using ThoughtWorks.QRCode.Codec.Data;
using System.Drawing;

using System.IO;

namespace WebQRC
{
    public partial class WebForm1 : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string temp2=Request.QueryString["url"];
            this.TextBox2.Text = Request.QueryString["id"];
            this.TextBox1.Text = temp2 + "&rid=" + this.TextBox2.Text;
            create_two(this.TextBox1.Text, this.TextBox2.Text);
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            create_two(this.TextBox1.Text,this.TextBox2.Text);
        }
        private void create_two(string nr,string name)
        {
            Bitmap bt;
            string enCodeString = nr;
            QRCodeEncoder qrCodeEncoder = new QRCodeEncoder();
            bt=qrCodeEncoder.Encode(enCodeString,Encoding.UTF8);//注意编码
            bool dx = File.Exists("~/image/" + name + ".jpg");
            if (dx)
            {
                File.Delete("~/image/" + name + ".jpg");
                bt.Save(Server.MapPath("~/image/" + name + ".jpg"));
                this.Image1.ImageUrl = "~/image/" + name + ".jpg";

            }
            else
            {
                bt.Save(Server.MapPath("~/image/" + name + ".jpg"));
                this.Image1.ImageUrl = "~/image/" + name + ".jpg";
            }
            
        }
    }
}