#include <openssl/decoder.h>
#include <openssl/pem.h>

#undef NDEBUG
#include <assert.h>

char PRIVATE_KEY_PEM[] = R"(
-----BEGIN PRIVATE KEY-----
MIICXQIBADCCAjUGByqGSM44BAEwggIoAoIBAQCZjYwtn1154ZOyMBsD8JRoWsob
D9t+qqqJnB/9EBwxjyzL7ynVCVSmWUupsSKbeI2cvt+A7FiOh1AWX11orgnp7rRE
ffFcF039dFhBDDZOAwjv/1cNzQSR9318LRS/jrRO17XDy11g6aot3baaPsVAF+Ro
giysMwEd5Kd5FRSqEf/rwUCJSygv005Fnaxu9xBn4zhrLPfmWJORAtZXTjIoHCxb
59lymjPu4gAQEeluzAi7kC2W2HZ6VCASVcFeOPQfIWVn6KCzEwAeTr/2nB8VdBVA
B5dJg7Gl8PJvvCL785vYVjOQLepyhQPnJ9MeDMuJvjJm/Cc6Iw7UEirkKoDFAh0A
2U3qJ86MKVo1oVEugVYK2gGu4pCKZUytXGS5JwKCAQAOy9yZKx2OHsc3QvF7vjgz
er3SEMnB8vn3d5errAzclps8UdjfEDOKoFXlAIfEdllhq1skQmtX3vGNhk2yJocQ
5NWtBp+xAgL1sKzbsCIxXwZvLo04kWdEzZRm1jZuJ2R0J8ARRz7awL0frvev5Qdr
LmUdA8PqmGgwWz4N++lXXbrkHJV4N0LM1sNeugAi2+A+zdpma8Bq41C2l6USn0MY
JfsT80oQBYwjwApxBGwFzcxy9NBdgNoePCs+3ZvxoKfmQHvoeLWJ/8sYyfAeOyC9
2iN7yCRubRygJQRJ5MXlto7mL5vKI6Joh14yNNMM+2+94GyTBmhDTnBuPUZJPfnN
BB8CHQDRdJyfnTW0n1DNDf/ppNav952+A4m/tFtvffNj
-----END PRIVATE KEY-----
)";

EVP_PKEY *load_dsa_pem(char *pem, size_t pem_size, int selection) {
  EVP_PKEY *key = nullptr;

  auto bio = BIO_new_mem_buf(pem, pem_size);
  assert(bio && "Failed to create private key buffer");

  auto ctx = OSSL_DECODER_CTX_new_for_pkey(&key, "PEM", nullptr, "DSA",
                                           selection, nullptr, nullptr);
  assert(ctx && "Failed to create decoder");

  auto ok = OSSL_DECODER_from_bio(ctx, bio);
  assert(ok && "Failed to decode");

  OSSL_DECODER_CTX_free(ctx);
  return key;
}

int main(int argc, char const **argv) {
  (void)argc;
  (void)argv;

  auto private_key = load_dsa_pem(PRIVATE_KEY_PEM, sizeof(PRIVATE_KEY_PEM),
                                  OSSL_KEYMGMT_SELECT_PRIVATE_KEY);

  auto file = BIO_new_file("out.pem", "w");
  PEM_write_bio_PrivateKey(file, private_key, nullptr, nullptr, 0, nullptr, nullptr);

  return 0;
}
