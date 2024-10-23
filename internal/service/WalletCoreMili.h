#ifndef __WalletCore_H__
#define __WalletCore_H__

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef const void TWString;

/**
* tss私钥创建
* @curve: ecdsa或者eddsa
* @session: 用户系统返回，格式：
*           普通托管创建： sessionid-salt
*           社交托管创建： sessionid-salt
* @preParam: 初始化时从后端拉取
* @mode: 枚举，托管trust, 社交恢复发起方origin, 社交恢复托管方backup, 升级为社交恢复upgrade
*        普通托管创建： 发起方trust
*        社交托管创建： 发起方origin，托管方backup
*        企业钱包创建2-2： user_v2
* 返回：私钥碎片json串，需要调用free释放，结构：{"status":true/false, "result":"", "error":""}
*/
extern const char* GoCreateMili23(const char* curve, const char* session, const char* preParam, const char* mode);

/**
* tss私钥重置
* @curve: 同上
* @session: 用户系统返回，格式：
*           普通托管重置： sessionId-oldSalt-userId-accountId-newSalt
*           社交托管重置： sessionId-oldSalt---newSalt
*           升级为社交托管：sessionId-oldSalt---newSalt
* @key: 本地私钥碎片，结构：{"Key": "...", "Pub": "..."}
* @mode: 枚举，托管trust, 社交恢复发起方origin, 社交恢复托管方backup, 升级为社交恢复upgrade
*        普通托管重置： 发起方trust
*        快速托管重置： 发起方trust_v2
*        社交托管c重置： 发起方origin，托管方backup
*        升级为社交托管：发起方upgrade，托管费backup
*        企业钱包邀请管理员：创建founder，管理员manager
*        企业钱包v2：邀请人manager_v2_invitor, 管理员manager_v2
* 返回： 私钥碎片json串，需要调用free释放，结构：{"status":true/false, "result":"", "error":""}
*/
extern const char* GoReshareMili23(const char* curve, const char* session, const char* key, const char* preParam, const char* mode);


/**
* @key: 本地单元，格式{"Key":"...", "Pub":["...","..."]}
* @offset: 后端返回
* 返回：{"Key":md5(managerKey), "Pub":managerKey.Pub}，和企业钱包普通成员的派生结果作用相同
*/
extern const char* GoDeriveMili23(const char* curve, const char* key, const char* offset);

/**
* 预签名，目前只支持ECDSA
* @session: 拼接格式：sessionId-salt[-offset](offset可选)
* 返回：预签名结果，结构：{"status":true/false, "result":"preSign", "error":""
*/
extern const char* GoOfflineSignMili23(const char* curve, const char* session, const char* key);

/**
 * 对十六进制数据签名，如果dapp传入数据未经转换就直接传入，可能直接转移资产
 * @curve: 同上
 * @key: 拼接字段，格式："mili:{}:{}{}".format(session, preSign, key)
 *       session拼接格式：sessionId-salt[-offset](offset可选)
 * @msg: 十六进制消息
 *
 */
extern const char* GoSignMili23(const char* curve, const char* key, const char* msg);

/*
* 私钥分片解密
* @userId: 用户Id
* @accountId: 私钥对应accountId
* @deKey: 密文
* 返回：解密结果
*/
extern const char* GoDecryptShareKey(const char* userId, const char* accountId, const char* enKey);

/*
* 私钥分片加密
* @userId: 同上
* @accountId: 同上
* @deKey: 明文
* 返回：加密结果
*/
extern const char* GoEncryptShareKey(const char* userId, const char* accountId, const char* deKey);

/*
* 加密明文私钥并格式化
* @key: 明文分片
* @salt: 加密盐
*/
extern const char* GoEncryptMiliKey(const char* key, const char* salt);

/*
* 设置请求环境变量(json)
* @params: {"server":"", "gw_server":"", "token":"", "secret":"", "cert":"", "cert_sn":"", "log_path":""}
*/
extern void GoSetRequestEnv(const char* jsonEnv);

/*
* 发送网关请求
* @method: POST/GET
* @header: 请求header，json字典
* @path: 请求路径
* @params: json结构[{"k":"参数名", "v":"参数值", "sign_it": true}], 如果一些参数不参与签名，sign_it为false
* @payload: 请求body，如果params传空，则解析payload并在其中添加sign后做为body
* 返回：请求结果，结构：{"status":true/false, "result":"", "error":""}
*/
extern const char* GoGwRequest(const char* method, const char* header, const char* path, const char* params, const char* payload);

/*
* 构建网关请求【将GwRequest拆分为构建、发送、解密三个步骤】
* @aesKey: 加密密钥
* @method: POST/GET
* @header: 同上
* @path: 同上
* @params: 同上
* @payload: 同上
* 返回：待发送POST请求的body
*/
extern const char* GoBuildRequest(const char* aesKey, const char* method, const char* header, const char* path, const char* params, const char* payload);

/*
* 解密网关返回结果
* @aesKey: 对应GoBuildRequest中的加密密钥
* @enResp: 网关返回data
* 返回：请求结果
*/
extern const char* GoLandResponse(const char* aesKey, const char* enResp);

/*
* 创建助记词钱包，返回助记词加密结果
* @strength: 助记词长度，一般12或者24
* @passphrase: 密码
*/
extern const char* CppCreateHDWallet(int strength, const char* passphrase);

/*
* 从助记词派生地址
* @wallet: 加密助记词
* @passphrase: 密码
* @coin: 币种(链)类型
* @index: 地址序号
*/
extern const char* CppDeriveAddressFromHDWallet(const char* wallet, const char* passphrase, int coin, int index);
/*
* 从助记词派生地址
* @txInput: json结构的交易输入
* @wallet: 加密助记词
* @passphrase: 密码
* @coin: 币种(链)类型
* @index: 地址序号
*/
extern TWString* CppJsonTransactionHDWallet(const char * txInput, const char* wallet, const char* passphrase, int coin, int index);
/*
* 地址创建
* @key: 本地私钥碎片
* @coinId: 链配置中的coinId
* 返回：地址
*/
extern const char* CppAddressCreateWithMiliKey(const char * key, int coinId, const char* hrp);

/*
* 获取公钥
* @key: 本地私钥碎片
* @coinId: 链配置中的coinId
* 返回：公钥hex串
*/
extern const char* CppPublicKeyWithMiliKey(const char * key, int coinId);

/*
* 地址校验
*/
extern bool CppAddressIsValid(const char* address, int coinId, const char* hrp);

/*
* 对任意消息签名
* @session: 拼接格式：sessionId-salt[-offset](offset可选)
* @msg: 签名消息
* 返回：十六进制签名结果，结构：{"status":true/false, "result":"", "error":""}
*/
extern TWString* CppSignMili23(const char* session, const char* key, const char* preSign, int coinId, const char* msg);

/*
* 构建可上链交易
* @session: 拼接格式：sessionId-salt[-offset](offset可选)
* @input: 交易输入参数，json结构
* 返回：十六进制交易，结构：{"status":true/false, "result":"", "error":""}
*/
extern TWString* CppJsonTransactionMili23(const char* session, const char* key, const char* preSign, int coinId, const char* input);

/*
* 获取预交易preRawTx
* @offset: 子钱包偏移或者空串
* @input: 交易输入参数，json结构
* 返回：未签名的十六进制交易，结构：{"status":true/false, "result":"", "error":""}
*/
extern TWString* CppJsonPreRawTx(const char* offset, const char* key, int coinId, const char* input);

/*
* UTXO plan
* @input: 交易输入参数，json结构
* 返回：{"amount":1000, "fee":100, "utxo_size":2}
*/
extern TWString* CppUtxoPlan(int coinId, const char* input);
/*
* 解析Evm合约数据
*/
extern TWString* CppDecodeEvmCall(const char* callData);

/*
* 操作TWString类型
*/
const char* TWStringUTF8Bytes(TWString* string);
size_t TWStringSize(TWString* string);
void TWStringDelete(TWString* string);

/*===============以下是链上信息获取接口==================*/
/*
chaindata_initChainConfig下发链配置，初始化SDK之后，需要先调用一下
chainConfig:是链配置数组的字符串
chainConfig例子：[{"chain_type":"EVM","chain":"ETH","proxy_key":"gasOracleETH","rpc_urls":["https://mainnet.infura.io/v3/9aa3d95b3bc440fa88ea12eaa4456161","https://web3os.tokenpocket.pro"],"proxy_cache_time":15}]
resolvers: json对象，链配置中获取，具体文档：https://tk3en79uf0.larksuite.com/docx/doxus5KstWG3LsebEHAdoj62vWb
返回结构：{"status":true/false, "result":"", "error":""}
*/
extern const char* chaindata_initChainConfig(const char* chainConfig, const char* resolvers);

/*
chaindata_getTransactionParams统一获取交易参数的接口
chain：哪条链
params:不同的链，传不同对象的json字符串
传入不同链的参数文档：https://tk3en79uf0.larksuite.com/docx/doxustISaUMqqdBwFNx4h1oPbge
*/
extern const char* chaindata_getTransactionParams(const char* chain, const char* params);

extern const char* chaindata_dappJson(const char* chain, const char* params);
/*
chaindata_getEIP1559TokenParams获取eip1559 token交易参数
handler：链名称
fromAddress：发送地址
toAddress：接受地址
tokenAddress：token地址
tpe:approve或者transfer
返回：交易参数，返回结果类型是map[string]interface{}的json字符串，value中可能也包含map结构
*/
extern const char* chaindata_getEIP1559TokenParams(const char* chain, const char* fromAddress, const char* toAddress, const char* tokenAddress, const char* tpe);

/*
chaindata_getTokenTxParams获取token交易参数
chain：链名称
fromAddress：发送地址
toAddress：接受地址
tokenAddress：token地址
tpe:approve或者transfer
返回：交易参数，返回结果类型是map[string]interface{}的json字符串，value中可能也包含map结构
*/
extern const char* chaindata_getTokenTxParams(const char* chain, const char* fromAddress, const char* toAddress, const char* tokenAddress, const char* tpe);

/*
chaindata_getTxParams获取交易参数
chain：链名称
fromAddress：发送地址
返回：交易参数，返回结果类型是map[string]interface{}的json的字符串，value中可能也包含map结构
*/
extern const char* chaindata_getTxParams(const char* chain, const char* fromAddress);

/*
chaindata_getSTCTxParams获取stc交易参数
chain：链名称
fromAddress：发送地址
toAddress：接受地址
publicKey: 用户的公钥
typeArgs：代币地址
返回：交易参数，返回结果类型是map[string]interface{}的json字符串，value中可能也包含map结构
*/
extern const char* chaindata_getSTCTxParams(const char* chain, const char* fromAddress, const char* toAddress, const char* publicKey, const char* typeArgs);

/*
chaindata_getEIP1559TxParams获取eip1559交易参数
chain：链名称
fromAddress：发送地址
返回：交易参数，返回结果类型是map[string]interface{}的json字符串，value中可能也包含map结构
*/
extern const char* chaindata_getEIP1559TxParams(const char* chain, const char* fromAddress);

/*
chaindata_getTokenType获取token信息
chain：链名称
tokenAddress：token地址的数组的json字符串
返回：token信息，返回结果类型是map[tokenAddress]tokenInfo的json字符串;tokenInfo是map[string]string
*/
extern const char* chaindata_getTokenType(const char* chain, const char* tokenAddress);

/*
chaindata_sendRawTransaction发送交易
chain：链名称
rawTx：交易数据
返回：交易hash
*/
extern const char* chaindata_sendRawTransaction(const char* chain, const char* rawTx);

/*
chaindata_sendDappRawTransaction发送交易
chain：链名称
rawTx：交易数据
返回：交易hash
*/
extern const char* chaindata_sendDappRawTransaction(const char* chain, const char* rawTx);

/*
chaindata_getTransaction获取交易信息
chain：链名称
address：当前用户的address
txHashs: 交易hash的数组的json字符串
返回：交易hash信息，返回结构是map[txhash]txInfo的json字符串;txInfo是map[string]string
*/
extern const char* chaindata_getTransaction(const char* chain, const char* address, const char* txHashs);

/*
chaindata_allBalance获取当前链的所有地址的所有币种的余额
chain：链名称
address：当前链的所有地址，结构是map[address]map[contract][decimals]的json字符串
返回：返回结构是map[address:map[{address:balance, token1:balance}]]的json字符串
*/
extern const char* chaindata_allBalance(const char* chain, const char* address);

/*
chaindata_getGasEstimate get gas estimated time
chain:那条链，现在暂只支持ETH
params:map[string]string{"gas_price":"8000000000"}格式的json字符串
*/
extern const char* chaindata_getGasEstimate(const char* chain, const char* params);

/*
chaindata_abiDecode 解析dapp交互data字段
contract: 合约地址
data: dapp rpc交互中data字段
*/
extern const char* chaindata_abiDecode(const char* chain, const char* contract, const char* data);

/*
chain_addressActive：判断当前地址的token地址时候有激活(主要是aptos)
chain: 链名称
address: 钱包地址
result:返回所有激活的token
*/
extern const char* chaindata_addressActive(const char* chain, const char* address);

/*
chaindata_getResourceActive：获取address下的token资源是否激活(solana使用)
chain: 链名称
resourceInfo: {"address":"","token_address":""}格式的json字符串
result:true:激活；false：没有激活
*/
extern const char* chaindata_getResourceActive(const char* chain, const char* resourceInfo);

/*
chaindata_getRpcURL：获取当前链的rpc(web端是在显示当前节点地方调用)
*/
extern const char* chaindata_getRpcURL(const char* chain);

/*
chaindata_detectNode 探测节点(web端是打开所有可用节点的时候调用，主动触发)
chain:链
nodeURL:节点
result:返回map,字段key是height，subTime
 */
extern const char* chaindata_detectNode(const char* chain, const char* nodeURL);

/*
chaindata_setRpc 设置当前节点(web端在rpc节点选择的时候去选择节点)
chain：链
nodeURL:节点
recordFlag：是否设置成最优节点 bool
 */
extern const char* chaindata_setRpc(const char* chain, const char* nodeURL, const char* recordFlag);

/*
chaindata_getSubTime 获取当前节点状况，网络延时，web在节点地方调用
chain：链
nodeURL:节点
result:返回map，字段key是sub_time，own_flag
 */
extern const char* chaindata_getSubTime(const char* chain, const char* nodeURL);

/*
chaindata_getNonce 获取nonce
chain：链
address:地址
*/
extern const char* chaindata_getNonce(const char* chain, const char* address);

/*
chaindata_getDirectTransfer 获取是否支持转nft(aptos nft用到)
chain：链
address:地址
*/
extern const char* chaindata_getDirectTransfer(const char* chain, const char* address);

/*
queryInfo: json object，内部字段如下：
query: 域名或地址
reverse: 是否反向解析， "true" or "false"
project: 查询项目
具体文档：https://tk3en79uf0.larksuite.com/docx/doxus5KstWG3LsebEHAdoj62vWb
*/
extern const char* chaindata_lookupDomain(const char* chain, const char* queryInfo);

/*
chaindata_identifyRiskAddress 转账风险地址判定
query: {"from_address": "", "to_address": ""}
*/
extern const char* chaindata_identifyRiskAddress(const char* chain, const char* query);

/*
chaindata_initCustomConfig 添加自定义链
chainConfig:是链配置数组的字符串
chainConfig例子：[{"type":"EVM","chain":"ETH","proxyKey":"gasOracleETH","rpcURLs":["https://mainnet.infura.io/v3/9aa3d95b3bc440fa88ea12eaa4456161","https://web3os.tokenpocket.pro"],"proxyCacheTime":15,"useNodeProxy":true,"chainId":1}]
*/
extern const char* chaindata_initCustomConfig(const char* chainConfig);

/*
chaindata_setOptimalURL 设置最优节点
*/
extern const char* chaindata_setOptimalURL(const char* chain);

/*
chaindata_getBlockHeader 获取块信息(trx/solana链)
*/
extern const char* chaindata_getBlockHeader(const char* chain);

#ifdef __cplusplus
}
#endif
#endif
