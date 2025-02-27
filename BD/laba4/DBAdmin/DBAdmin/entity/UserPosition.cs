using System.ComponentModel.DataAnnotations.Schema;

namespace DBAdmin.entity
{
    [Table("user_positions")]
    public class UserPosition
    {
        [Column("id")]
        public int Id { get; set; }

        [Column("title")]
        public string Title { get; set; }
    }
}
