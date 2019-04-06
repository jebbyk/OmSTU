using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(blogDBApp.Startup))]
namespace blogDBApp
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
