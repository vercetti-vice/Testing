using Microsoft.EntityFrameworkCore;

namespace WebApplication1.Models
{
    public class AppContext : DbContext
    {
        public AppContext(DbContextOptions<AppContext> options)
            : base(options)
        {
            Database.EnsureCreated();
        }
        
        public DbSet<FileModel> Files { get; set; }
    }
}