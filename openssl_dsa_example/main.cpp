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

char PUBLIC_KEY_PEM[] = R"(
-----BEGIN PUBLIC KEY-----
MIIDQjCCAjUGByqGSM44BAEwggIoAoIBAQCZjYwtn1154ZOyMBsD8JRoWsobD9t+
qqqJnB/9EBwxjyzL7ynVCVSmWUupsSKbeI2cvt+A7FiOh1AWX11orgnp7rREffFc
F039dFhBDDZOAwjv/1cNzQSR9318LRS/jrRO17XDy11g6aot3baaPsVAF+Rogiys
MwEd5Kd5FRSqEf/rwUCJSygv005Fnaxu9xBn4zhrLPfmWJORAtZXTjIoHCxb59ly
mjPu4gAQEeluzAi7kC2W2HZ6VCASVcFeOPQfIWVn6KCzEwAeTr/2nB8VdBVAB5dJ
g7Gl8PJvvCL785vYVjOQLepyhQPnJ9MeDMuJvjJm/Cc6Iw7UEirkKoDFAh0A2U3q
J86MKVo1oVEugVYK2gGu4pCKZUytXGS5JwKCAQAOy9yZKx2OHsc3QvF7vjgzer3S
EMnB8vn3d5errAzclps8UdjfEDOKoFXlAIfEdllhq1skQmtX3vGNhk2yJocQ5NWt
Bp+xAgL1sKzbsCIxXwZvLo04kWdEzZRm1jZuJ2R0J8ARRz7awL0frvev5QdrLmUd
A8PqmGgwWz4N++lXXbrkHJV4N0LM1sNeugAi2+A+zdpma8Bq41C2l6USn0MYJfsT
80oQBYwjwApxBGwFzcxy9NBdgNoePCs+3ZvxoKfmQHvoeLWJ/8sYyfAeOyC92iN7
yCRubRygJQRJ5MXlto7mL5vKI6Joh14yNNMM+2+94GyTBmhDTnBuPUZJPfnNA4IB
BQACggEABO/uw+jUy7+B2l0D6qM6gBTVU0aD/e2vFLVgCALujhzscfIVVrnhzHWX
5eoF4VoN0/Xl+ng8eVQf0xloLAPdI+Ieo7cTtt6eQHxa65s94n2+Ap0jMRHdTRWb
pzPhviuFgJbX2YDPIznEBihJLsQ0V9aLFb7ij9m1Bqgad5Bp71CoheoL3JQoFzeX
F7//aMcfJQTi1c/bjuhcYZ5rB4YiC0eYeBNlwhIaiJuqoHX3UHEzU9Vin44x/JOh
dR+vu5sfDaISR5z9sPcHycJ6exOsa5UbMlvKHlZmSL/gmADvQxHq3CvxI6fUMo9Y
Iih3cwF6h2XYOMvO2XUmMQZblpxU4A==
-----END PUBLIC KEY-----
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
#if 0
  auto public_key = load_dsa_pem(PUBLIC_KEY_PEM, sizeof(PUBLIC_KEY_PEM),
                                 OSSL_KEYMGMT_SELECT_PUBLIC_KEY);
#endif

  uint8_t data[] = "AAAAAAAAAAAAAAAAAAAA";
  size_t data_size = 20;

  uint8_t sig[128] = {0};
  size_t sig_size = sizeof(sig);

  auto sign_ctx = EVP_PKEY_CTX_new(private_key, nullptr);
  EVP_PKEY_sign_init(sign_ctx);
  EVP_PKEY_sign(sign_ctx, sig, &sig_size, data, data_size);

  puts(OPENSSL_buf2hexstr(sig, sig_size));

  return 0;
}
