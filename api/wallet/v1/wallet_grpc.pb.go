// Code generated by protoc-gen-go-grpc. DO NOT EDIT.
// versions:
// - protoc-gen-go-grpc v1.5.1
// - protoc             v5.26.1
// source: api/wallet/v1/wallet.proto

package v1

import (
	context "context"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
)

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
// Requires gRPC-Go v1.64.0 or later.
const _ = grpc.SupportPackageIsVersion9

const (
	Wallet_CreateWallet_FullMethodName    = "/api.wallet.v1.Wallet/CreateWallet"
	Wallet_GetAddress_FullMethodName      = "/api.wallet.v1.Wallet/GetAddress"
	Wallet_SignTransaction_FullMethodName = "/api.wallet.v1.Wallet/SignTransaction"
)

// WalletClient is the client API for Wallet service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://pkg.go.dev/google.golang.org/grpc/?tab=doc#ClientConn.NewStream.
type WalletClient interface {
	CreateWallet(ctx context.Context, in *CreateWalletRequest, opts ...grpc.CallOption) (*CreateWalletReply, error)
	GetAddress(ctx context.Context, in *GetAddressRequest, opts ...grpc.CallOption) (*GetAddressReply, error)
	SignTransaction(ctx context.Context, in *SignTransactionRequest, opts ...grpc.CallOption) (*SignTransactionReply, error)
}

type walletClient struct {
	cc grpc.ClientConnInterface
}

func NewWalletClient(cc grpc.ClientConnInterface) WalletClient {
	return &walletClient{cc}
}

func (c *walletClient) CreateWallet(ctx context.Context, in *CreateWalletRequest, opts ...grpc.CallOption) (*CreateWalletReply, error) {
	cOpts := append([]grpc.CallOption{grpc.StaticMethod()}, opts...)
	out := new(CreateWalletReply)
	err := c.cc.Invoke(ctx, Wallet_CreateWallet_FullMethodName, in, out, cOpts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *walletClient) GetAddress(ctx context.Context, in *GetAddressRequest, opts ...grpc.CallOption) (*GetAddressReply, error) {
	cOpts := append([]grpc.CallOption{grpc.StaticMethod()}, opts...)
	out := new(GetAddressReply)
	err := c.cc.Invoke(ctx, Wallet_GetAddress_FullMethodName, in, out, cOpts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *walletClient) SignTransaction(ctx context.Context, in *SignTransactionRequest, opts ...grpc.CallOption) (*SignTransactionReply, error) {
	cOpts := append([]grpc.CallOption{grpc.StaticMethod()}, opts...)
	out := new(SignTransactionReply)
	err := c.cc.Invoke(ctx, Wallet_SignTransaction_FullMethodName, in, out, cOpts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// WalletServer is the server API for Wallet service.
// All implementations must embed UnimplementedWalletServer
// for forward compatibility.
type WalletServer interface {
	CreateWallet(context.Context, *CreateWalletRequest) (*CreateWalletReply, error)
	GetAddress(context.Context, *GetAddressRequest) (*GetAddressReply, error)
	SignTransaction(context.Context, *SignTransactionRequest) (*SignTransactionReply, error)
	mustEmbedUnimplementedWalletServer()
}

// UnimplementedWalletServer must be embedded to have
// forward compatible implementations.
//
// NOTE: this should be embedded by value instead of pointer to avoid a nil
// pointer dereference when methods are called.
type UnimplementedWalletServer struct{}

func (UnimplementedWalletServer) CreateWallet(context.Context, *CreateWalletRequest) (*CreateWalletReply, error) {
	return nil, status.Errorf(codes.Unimplemented, "method CreateWallet not implemented")
}
func (UnimplementedWalletServer) GetAddress(context.Context, *GetAddressRequest) (*GetAddressReply, error) {
	return nil, status.Errorf(codes.Unimplemented, "method GetAddress not implemented")
}
func (UnimplementedWalletServer) SignTransaction(context.Context, *SignTransactionRequest) (*SignTransactionReply, error) {
	return nil, status.Errorf(codes.Unimplemented, "method SignTransaction not implemented")
}
func (UnimplementedWalletServer) mustEmbedUnimplementedWalletServer() {}
func (UnimplementedWalletServer) testEmbeddedByValue()                {}

// UnsafeWalletServer may be embedded to opt out of forward compatibility for this service.
// Use of this interface is not recommended, as added methods to WalletServer will
// result in compilation errors.
type UnsafeWalletServer interface {
	mustEmbedUnimplementedWalletServer()
}

func RegisterWalletServer(s grpc.ServiceRegistrar, srv WalletServer) {
	// If the following call pancis, it indicates UnimplementedWalletServer was
	// embedded by pointer and is nil.  This will cause panics if an
	// unimplemented method is ever invoked, so we test this at initialization
	// time to prevent it from happening at runtime later due to I/O.
	if t, ok := srv.(interface{ testEmbeddedByValue() }); ok {
		t.testEmbeddedByValue()
	}
	s.RegisterService(&Wallet_ServiceDesc, srv)
}

func _Wallet_CreateWallet_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(CreateWalletRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(WalletServer).CreateWallet(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: Wallet_CreateWallet_FullMethodName,
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(WalletServer).CreateWallet(ctx, req.(*CreateWalletRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _Wallet_GetAddress_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(GetAddressRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(WalletServer).GetAddress(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: Wallet_GetAddress_FullMethodName,
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(WalletServer).GetAddress(ctx, req.(*GetAddressRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _Wallet_SignTransaction_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(SignTransactionRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(WalletServer).SignTransaction(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: Wallet_SignTransaction_FullMethodName,
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(WalletServer).SignTransaction(ctx, req.(*SignTransactionRequest))
	}
	return interceptor(ctx, in, info, handler)
}

// Wallet_ServiceDesc is the grpc.ServiceDesc for Wallet service.
// It's only intended for direct use with grpc.RegisterService,
// and not to be introspected or modified (even as a copy)
var Wallet_ServiceDesc = grpc.ServiceDesc{
	ServiceName: "api.wallet.v1.Wallet",
	HandlerType: (*WalletServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "CreateWallet",
			Handler:    _Wallet_CreateWallet_Handler,
		},
		{
			MethodName: "GetAddress",
			Handler:    _Wallet_GetAddress_Handler,
		},
		{
			MethodName: "SignTransaction",
			Handler:    _Wallet_SignTransaction_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "api/wallet/v1/wallet.proto",
}
