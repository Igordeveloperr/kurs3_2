using DBAdmin.entity;
using Microsoft.EntityFrameworkCore;
using static Microsoft.EntityFrameworkCore.DbLoggerCategory.Database;

namespace DBAdmin.context
{
    public class AppDbContext : DbContext
    {
        public DbSet<User> Users { get; set; } = null!;
        public DbSet<UserPosition> UserPositions { get; set; } = null!;
        public DbSet<RequestsClientService> RequestsClientServices { get; set; } = null!;
        public DbSet<RequestServiceToPosition> RequestServiceToPositions { get; set; } = null!;
        public DbSet<ApartmentBuilding> ApartmentBuildings { get; set; } = null!;
        public DbSet<ApartmentBuildingObjectType> ApartmentBuildingObjectTypes { get; set; } = null!;
        public DbSet<RequestsService> RequestsServices { get; set; } = null!;
        public DbSet<RequestClientApplication> RequestClientApplications { get; set; } = null!;
        public DbSet<RequestPerformerService> RequestPerformerServices { get; set; } = null!;

        public AppDbContext()
        {
            Database.EnsureCreated();
        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseNpgsql("Host=localhost;Port=5432;Database=mydb;Username=postgres;Password=13579");
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            // RequestServiceToPosition -> RequestClientService
            modelBuilder.Entity<RequestServiceToPosition>()
                .HasOne(r => r.Service)
                .WithMany()
                .HasForeignKey(r => r.ServiceId);

            // RequestServiceToPosition -> UserPosition
            modelBuilder.Entity<RequestServiceToPosition>()
                .HasOne(r => r.Position)
                .WithMany()
                .HasForeignKey(r => r.PositionId);

            //RequestsService -> RequestServiceToPosition
            modelBuilder.Entity<RequestsService>()
                .HasOne(r => r.ServiceToPosition)
                .WithMany()
                .HasForeignKey(r => r.ServiceId);

            //RequestsService -> RequestClientApplication
            modelBuilder.Entity<RequestsService>()
                .HasOne(r => r.ClientApplication)
                .WithMany()
                .HasForeignKey(r => r.RequestId);

            // RequestClientApplication -> User
            modelBuilder.Entity<RequestClientApplication>()
                .HasOne(r => r.Client)
                .WithMany()
                .HasForeignKey(r => r.ClientId);

            // RequestClientApplication -> ApartmentBuildingObjectType
            modelBuilder.Entity<RequestClientApplication>()
                .HasOne(r => r.ObjectType)
                .WithMany()
                .HasForeignKey(r => r.ObjectTypeId);

            // RequestClientApplication -> ApartmentBuilding
            modelBuilder.Entity<RequestClientApplication>()
                .HasOne(r => r.Apartment)
                .WithMany()
                .HasForeignKey(r => r.ObjectNumId);

            // RequestPerformerService -> RequestClientApplication
            modelBuilder.Entity<RequestPerformerService>()
                .HasOne(r => r.ClientApplication)
                .WithMany()
                .HasForeignKey(r => r.RequestId);

            // RequestPerformerService -> RequestsService
            modelBuilder.Entity<RequestPerformerService>()
                .HasOne(r => r.RequestService)
                .WithMany()
                .HasForeignKey(r => r.ServiceOnRequestId);

            // RequestPerformerService -> User
            modelBuilder.Entity<RequestPerformerService>()
                .HasOne(r => r.Implementer)
                .WithMany()
                .HasForeignKey(r => r.ImplementerId);
        }
    }
}
