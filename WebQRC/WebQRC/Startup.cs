using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(WebQRC.Startup))]
namespace WebQRC
{
    public partial class Startup {
        public void Configuration(IAppBuilder app) {
            ConfigureAuth(app);
        }
    }
}
