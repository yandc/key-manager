package data

import (
	"key-manager/internal/conf"
	"key-manager/internal/data/models"

	"github.com/go-kratos/kratos/v2/log"
	"github.com/go-redis/redis/v8"
	"github.com/google/wire"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
)

// ProviderSet is data providers.
var ProviderSet = wire.NewSet(NewData)

// Data .
type Data struct {
	// TODO wrapped database client
	DB       *gorm.DB
	RedisCli *redis.Client
	Log      *log.Helper
}

// NewData .
func NewData(c *conf.Data, logger log.Logger) (*Data, func(), error) {
	cleanup := func() {
		log.NewHelper(logger).Info("closing the data resources")
	}
	log := log.NewHelper(log.With(logger, "module", "data/gorm"))
	db, err := gorm.Open(postgres.Open(c.Database.Source), &gorm.Config{})
	if err != nil {
		log.Fatalf("failed opening connection to postgres: %v", err)
	}
	if err := db.AutoMigrate(&models.Wallet{}, &models.Address{}, &models.TransactionSignRecord{}); err != nil {
		log.Fatal(err)
	}

	cli := redis.NewClient(&redis.Options{
		Addr: c.Redis.Addr,
		DB:   int(c.Redis.Db),
	})

	return &Data{DB: db, RedisCli: cli, Log: log}, cleanup, nil
}
