package models

type Wallet struct {
	BaseModel
	Name    string `gorm:"index:,unique"`
	Entropy string
}

type Address struct {
	BaseModel
	WalletName   string
	CoinType     uint32
	AddressIndex uint32
	Address      string `gorm:"index:,unique"`
}

type TransactionSignRecord struct {
	BaseModel
	WalletName string
	Address    string
	TxInput    string
	RawTx      string
}
