package data

import (
	"key-manager/internal/conf"
	"key-manager/internal/data/models"

	"github.com/go-kratos/kratos/v2/log"
	"github.com/google/wire"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
)

// ProviderSet is data providers.
var ProviderSet = wire.NewSet(NewData)

// Data .
type Data struct {
	// TODO wrapped database client
	DB  *gorm.DB
	Log *log.Helper
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
	return &Data{DB: db, Log: log}, cleanup, nil
}
