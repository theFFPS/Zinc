#pragma once

#include <string>
#include <optional>
#include <vector>

namespace zinc {

struct ZincKeyringPackageKey {
    enum class KeyType : int {
        SymKey, PrivateKey, PublicKey
    } m_type;
    std::string m_keyId;
    std::string m_keyName;
    std::string m_keyVersion;
    std::string m_keyDescription;
    std::string m_keyAlgorithm;
    std::vector<std::string> m_keyAuthors;
    enum class KeyUsage : int {
        SignatureSigning, SignatureVerification, Encrypt, Decrypt
    };
    std::vector<KeyUsage> m_allowedUsages;
    std::optional<std::string> m_keyChecksum; // if key encryption is enabled write checksum of decrypted data
    std::optional<std::string> m_keyTrustProvider; // url to signature keyring
    std::optional<std::string> m_oppositeKeyId;
    std::vector<unsigned char> m_keyData; // encrypted data, also contains key expiration time
    std::vector<unsigned char> m_keySignature; // if no trust provider detected => use default signature keyring
};
struct ZincKeyringPackage {
    enum class KeyringType : int {
        MasterKeyring, SignatureVerificationKeyring, SignatureSigningKeyring, AuthKeyring, Certifcate, CustomKeyring
    } m_keyringType = KeyringType::CustomKeyring;
    std::string m_keyringId;
    std::string m_keyringName;
    std::string m_keyringVersion;
    std::string m_keyringDescription;
    std::vector<std::string> m_keyringAuthors;
    enum class KeyringEncryptionType : int {
        TPM2, MasterKey, Password
    };
    std::optional<KeyringEncryptionType> m_keyringEncryptionType = KeyringEncryptionType::Password;
    std::vector<ZincKeyringPackageKey> m_keyringData;
};

}