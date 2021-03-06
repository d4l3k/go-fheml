#include "seal.h"
#include "seal/seal.h"

SEALEncryptionParameters SEALEncryptionParametersBFV(void) {
  auto* params = new seal::EncryptionParameters(seal::scheme_type::BFV);
  params->set_poly_modulus_degree(2048);
  params->set_coeff_modulus(seal::coeff_modulus_128(2048));
  params->set_plain_modulus(1 << 8);
  return (void*)params;
}

SEALEncryptionParameters SEALEncryptionParametersCKKS(void) {
  auto* params = new seal::EncryptionParameters(seal::scheme_type::CKKS);
  params->set_poly_modulus_degree(16384);
  params->set_coeff_modulus(seal::coeff_modulus_128(16384));
  /*
	params->set_coeff_modulus({
      seal::small_mods_40bit(0), seal::small_mods_40bit(1),
			seal::small_mods_40bit(2), seal::small_mods_40bit(3) });
      */
  return (void*)params;
}

void SEALEncryptionParametersDelete(SEALEncryptionParameters p) {
  delete static_cast<seal::EncryptionParameters*>(p);
}

SEALContext SEALContextInit(SEALEncryptionParameters p) {
  auto* params = static_cast<seal::EncryptionParameters*>(p);
  auto ctx = seal::SEALContext::Create(*params);
  return (void*)new std::shared_ptr<seal::SEALContext>(std::move(ctx));
}

void SEALContextDelete(SEALContext c) {
  delete static_cast<std::shared_ptr<seal::SEALContext>*>(c);
}

SEALKeyGenerator SEALKeyGeneratorInit(SEALContext c) {
  auto* ctx = static_cast<std::shared_ptr<seal::SEALContext>*>(c);
  return (void*)new seal::KeyGenerator(*ctx);
}

void SEALKeyGeneratorDelete(SEALKeyGenerator g) {
  delete static_cast<seal::KeyGenerator*>(g);
}

SEALPublicKey SEALKeyGeneratorPublicKey(SEALKeyGenerator g) {
  auto* generator = static_cast<seal::KeyGenerator*>(g);
  auto key = generator->public_key();
  return (void*)new seal::PublicKey(key);
}

SEALSecretKey SEALKeyGeneratorSecretKey(SEALKeyGenerator g) {
  auto* generator = static_cast<seal::KeyGenerator*>(g);
  auto key = generator->secret_key();
  return (void*)new seal::SecretKey(key);
}

SEALSecretKey SEALKeyGeneratorRelinKeys(SEALKeyGenerator g,
                                        int decomposition_bit_count, int num) {
  auto* generator = static_cast<seal::KeyGenerator*>(g);
  auto key = generator->relin_keys(decomposition_bit_count, num);
  return (void*)new seal::RelinKeys(key);
}

void SEALPublicKeyDelete(SEALPublicKey k) {
  delete static_cast<seal::PublicKey*>(k);
}

void SEALSecretKeyDelete(SEALSecretKey k) {
  delete static_cast<seal::SecretKey*>(k);
}

void SEALRelinKeysDelete(SEALRelinKeys k) {
  delete static_cast<seal::RelinKeys*>(k);
}

SEALEncryptor SEALEncryptorInit(SEALContext c, SEALPublicKey k) {
  auto* ctx = static_cast<std::shared_ptr<seal::SEALContext>*>(c);
  auto* key = static_cast<seal::PublicKey*>(k);
  return (void*)new seal::Encryptor(*ctx, *key);
}

void SEALEncryptorDelete(SEALEncryptor k) {
  delete static_cast<seal::Encryptor*>(k);
}

void SEALCiphertextDelete(SEALCiphertext k) {
  delete static_cast<seal::Ciphertext*>(k);
}

SEALCiphertext SEALCiphertextCopy(SEALCiphertext k) {
  auto* c = static_cast<seal::Ciphertext*>(k);
  return (void*)new seal::Ciphertext(*c);
}

double SEALCiphertextScale(SEALCiphertext k) {
  auto* c = static_cast<seal::Ciphertext*>(k);
  return c->scale();
}

SEALParmsID SEALCiphertextParmsID(SEALCiphertext k) {
  auto* c = static_cast<seal::Ciphertext*>(k);
  return (void*)new seal::parms_id_type(c->parms_id());
}

void SEALParmsIDDelete(SEALParmsID k) {
  delete static_cast<seal::parms_id_type*>(k);
}

int SEALParmsIDEq(SEALParmsID aptr, SEALParmsID bptr) {
  auto* a = static_cast<seal::parms_id_type*>(aptr);
  auto* b = static_cast<seal::parms_id_type*>(bptr);
  return *a == *b;
}

SEALCiphertext SEALEncryptorEncrypt(SEALEncryptor k, SEALPlaintext p) {
  auto* e = static_cast<seal::Encryptor*>(k);
  auto* pl = static_cast<seal::Plaintext*>(p);
  seal::Ciphertext encrypted;
  e->encrypt(*pl, encrypted);
  return (void*)new seal::Ciphertext(encrypted);
}

SEALEvaluator SEALEvaluatorInit(SEALContext c) {
  auto* ctx = static_cast<std::shared_ptr<seal::SEALContext>*>(c);
  return (void*)new seal::Evaluator(*ctx);
}

void SEALEvaluatorDelete(SEALEvaluator k) {
  delete static_cast<seal::Evaluator*>(k);
}

void SEALEvaluatorSquareInplace(SEALEvaluator k, SEALCiphertext cptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* c = static_cast<seal::Ciphertext*>(cptr);
  e->square_inplace(*c);
}

void SEALEvaluatorNegateInplace(SEALEvaluator k, SEALCiphertext cptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* c = static_cast<seal::Ciphertext*>(cptr);
  e->negate_inplace(*c);
}

void SEALEvaluatorAddInplace(SEALEvaluator k, SEALCiphertext aptr,
                             SEALCiphertext bptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::Ciphertext*>(bptr);
  e->add_inplace(*a, *b);
}

void SEALEvaluatorAddPlainInplace(SEALEvaluator k, SEALCiphertext aptr,
                                  SEALPlaintext bptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::Plaintext*>(bptr);
  e->add_plain_inplace(*a, *b);
}

void SEALEvaluatorSubInplace(SEALEvaluator k, SEALCiphertext aptr,
                             SEALCiphertext bptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::Ciphertext*>(bptr);
  e->sub_inplace(*a, *b);
}

void SEALEvaluatorSubPlainInplace(SEALEvaluator k, SEALCiphertext aptr,
                                  SEALPlaintext bptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::Plaintext*>(bptr);
  e->sub_plain_inplace(*a, *b);
}

void SEALEvaluatorMultiplyInplace(SEALEvaluator k, SEALCiphertext aptr,
                                  SEALCiphertext bptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::Ciphertext*>(bptr);
  e->multiply_inplace(*a, *b);
}

void SEALEvaluatorMultiplyPlainInplace(SEALEvaluator k, SEALCiphertext aptr,
                                       SEALPlaintext bptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::Plaintext*>(bptr);
  e->multiply_plain_inplace(*a, *b);
}

void SEALEvaluatorRelinearizeInplace(SEALEvaluator k, SEALCiphertext aptr,
                                     SEALRelinKeys rptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::RelinKeys*>(rptr);
  e->relinearize_inplace(*a, *b);
}

void SEALEvaluatorExponentiateInplace(SEALEvaluator k, SEALCiphertext aptr,
                                      uint64_t power, SEALRelinKeys rptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* b = static_cast<seal::RelinKeys*>(rptr);
  e->exponentiate_inplace(*a, power, *b);
}

void SEALEvaluatorRescaleToNextInplace(SEALEvaluator k, SEALCiphertext aptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  e->rescale_to_next_inplace(*a);
}

void SEALEvaluatorRescaleToInplace(SEALEvaluator k, SEALCiphertext aptr, SEALParmsID pptr) {
  auto* e = static_cast<seal::Evaluator*>(k);
  auto* a = static_cast<seal::Ciphertext*>(aptr);
  auto* p = static_cast<seal::parms_id_type*>(pptr);
  auto pool = seal::MemoryManager::GetPool();
  if (!pool) {
    throw std::invalid_argument("what the fuck");
  }
  e->rescale_to_inplace(*a, *p, pool);
}

SEALDecryptor SEALDecryptorInit(SEALContext c, SEALSecretKey k) {
  auto* ctx = static_cast<std::shared_ptr<seal::SEALContext>*>(c);
  auto* key = static_cast<seal::SecretKey*>(k);
  return (void*)new seal::Decryptor(*ctx, *key);
}

void SEALDecryptorDelete(SEALDecryptor k) {
  delete static_cast<seal::Decryptor*>(k);
}

SEALPlaintext SEALDecryptorDecrypt(SEALDecryptor k, SEALCiphertext c) {
  auto* d = static_cast<seal::Decryptor*>(k);
  auto* ciphertext = static_cast<seal::Ciphertext*>(c);
  seal::Plaintext plain;
  d->decrypt(*ciphertext, plain);
  return (void*)new seal::Plaintext(plain);
}

SEALDecryptor SEALBinaryFractionalEncoderInit(SEALEncryptionParameters params) {
  auto* p = static_cast<seal::EncryptionParameters*>(params);
  return (void*)new seal::BinaryFractionalEncoder(
      p->plain_modulus(), p->poly_modulus_degree(),
      p->poly_modulus_degree() / 2 - 1, p->poly_modulus_degree() / 2 - 1);
}

void SEALBinaryFractionalEncoderDelete(SEALBinaryFractionalEncoder k) {
  delete static_cast<seal::BinaryFractionalEncoder*>(k);
}

SEALPlaintext SEALBinaryFractionalEncoderEncode(SEALBinaryFractionalEncoder k,
                                                double a) {
  auto* e = static_cast<seal::BinaryFractionalEncoder*>(k);
  return (void*)new seal::Plaintext(e->encode(a));
}

double SEALBinaryFractionalEncoderDecode(SEALBinaryFractionalEncoder k,
                                         SEALPlaintext a) {
  auto* e = static_cast<seal::BinaryFractionalEncoder*>(k);
  auto* p = static_cast<seal::Plaintext*>(a);
  return e->decode(*p);
}

void SEALPlaintextDelete(SEALPlaintext k) {
  delete static_cast<seal::Plaintext*>(k);
}

SEALCKKSEncoder SEALCKKSEncoderInit(SEALContext c) {
  auto* ctx = static_cast<std::shared_ptr<seal::SEALContext>*>(c);
  return (void*)new seal::CKKSEncoder(*ctx);
}

void SEALCKKSEncoderDelete(SEALCKKSEncoder k) {
  delete static_cast<seal::CKKSEncoder*>(k);
}

SEALPlaintext SEALCKKSEncoderEncode(SEALCKKSEncoder k, double num,
                                    SEALParmsID pidptr, double scale) {
  auto* e = static_cast<seal::CKKSEncoder*>(k);
  std::vector<double> data{num};
  seal::Plaintext p;
  if (pidptr != nullptr) {
    auto* pid = static_cast<seal::parms_id_type*>(pidptr);
    e->encode(data, *pid, scale, p);
  } else {
    e->encode(data, scale, p);
  }
  return (void*)new seal::Plaintext(p);
}

double SEALCKKSEncoderDecode(SEALCKKSEncoder k, SEALPlaintext p) {
  auto* e = static_cast<seal::CKKSEncoder*>(k);
  auto* plain = static_cast<seal::Plaintext*>(p);
  std::vector<double> data(1);
  e->decode(*plain, data);
  return data.at(0);
}
