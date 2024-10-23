package service

/*
#cgo CFLAGS: -I.
#cgo LDFLAGS: -L./libs -lwallet_core -lgo_mili -Wl,-rpath,libs
#include "WalletCoreMili.h"
*/
import "C"

import (
	"context"
	"encoding/json"
	"errors"
	"unsafe"

	pb "key-manager/api/wallet/v1"
	"key-manager/internal/data"
	"key-manager/internal/data/models"
)

type WalletService struct {
	pb.UnimplementedWalletServer
	data *data.Data
}

type SignResult struct {
	TxId   string `json:"txid"`
	Result string `json:"result"`
	Status bool   `json:"status"`
	Error  string `json:"error"`
}

func GetCppString(cppStr unsafe.Pointer) string {
	goStr := C.GoString(C.TWStringUTF8Bytes(cppStr))
	C.TWStringDelete(cppStr)
	return goStr
}

func NewWalletService(data *data.Data) *WalletService {
	return &WalletService{data: data}
}

func (s *WalletService) CreateWallet(ctx context.Context, req *pb.CreateWalletRequest) (*pb.CreateWalletReply, error) {
	entropy := C.GoString(C.CppCreateHDWallet(C.int(req.Strength), C.CString(req.Passphrase)))
	wallet := models.Wallet{
		Name:    req.Name,
		Entropy: entropy,
	}
	err := s.data.DB.Save(wallet).Error
	if err != nil {
		s.data.Log.Error("save wallet error: ", err, req.Name)
		return nil, err
	}
	return &pb.CreateWalletReply{Wallet: entropy}, nil
}
func (s *WalletService) GetAddress(ctx context.Context, req *pb.GetAddressRequest) (*pb.GetAddressReply, error) {
	var wallet *models.Wallet
	if err := s.data.DB.Where("name = ?", req.WalletName).First(&wallet).Error; err != nil {
		return nil, err
	}
	a := C.GoString(C.CppDeriveAddressFromHDWallet(C.CString(wallet.Entropy), C.CString(req.Passphrase), C.int(req.CoinType), C.int(req.AddressIndex)))
	address := models.Address{
		WalletName:   req.WalletName,
		CoinType:     req.CoinType,
		AddressIndex: req.AddressIndex,
		Address:      a,
	}
	err := s.data.DB.Save(address).Error
	if err != nil {
		s.data.Log.Error("derive address error: ", err, req.WalletName, req.CoinType, req.AddressIndex)
		return nil, err
	}
	return &pb.GetAddressReply{Address: a}, nil
}
func (s *WalletService) SignTransaction(ctx context.Context, req *pb.SignTransactionRequest) (*pb.SignTransactionReply, error) {
	var address *models.Address
	var wallet *models.Wallet
	if err := s.data.DB.Where("address = ?", req.Address).First(&address).Error; err != nil {
		return nil, err
	}
	if err := s.data.DB.Where("name = ?", address.WalletName).First(&wallet).Error; err != nil {
		return nil, err
	}

	trans := unsafe.Pointer(C.CppJsonTransactionHDWallet(C.CString(req.TxInput), C.CString(wallet.Entropy), C.CString(req.Passphrase), C.int(address.CoinType), C.int(address.AddressIndex)))
	rawTx := GetCppString(trans)
	var signed SignResult
	if err := json.Unmarshal([]byte(rawTx), &signed); err != nil {
		return nil, err
	}
	if !signed.Status {
		return nil, errors.New(signed.Error)
	}
	return &pb.SignTransactionReply{RawTx: signed.Result, TxId: signed.TxId}, nil
}
