/* Copyright (c) 2016, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <openssl/chacha.h>
#include <unittest.h>

#include "all_tests.h"

namespace {

const char* kKey =
    "98bef1469be7269837a45bfbc92a5a6ac762507cf96443bf33b96b1bd4c6f8f6";

const char* kNonce = "44e792d63335abb1582e9253";

const uint32_t kCounter = 42;

const char* kInput =
    "5828d530362c605529f8e18cae151526f23a73a0f312a3885f2b74233dc90523c654491e44"
    "8814d9da3715dcb7e423b39d7e166835fc026dcc8ae5dd5fe4d2566f129c9c7d6a3848bddf"
    "d9ac1ba24dc543043cd799e1a7139c51c26df9cf073be4bf93a3a9b4c5f01ae48d5fc6c47c"
    "697ade1ac1c9cfc24e7a252c32e917ba68f1375d628446f5807f1a71f7be724bb81cfe3ebd"
    "ae0d730d874a31c33d466fb3d76be3b870178e7a6a0ebfa8bc2bdbfa4fb62620ee63f06d26"
    "ac6a18376e5981d160e640d56d68ba8b654af9f1ae56248fe38ee77e6fcf92dfa9753ad62e"
    "1caff2d68b39add25dfbd7df05570df7f68f2d14b04e1a3c7704cd3c5c5852106fcf5c03c8"
    "5f852b058260dacccdd688bfc010b36f545442bc4b77214dee8745064c6038d27e1d306c55"
    "f038801cde3dea683ef63e59cf0d08ae8c020bc1726ab46df3f7b3ef3ab106f2f4d6697b3e"
    "a216313179b633a9ca8aa8bef3e93828d1e13b4e2e4735a461141e422c4955eae3b3ce39d3"
    "b3ef4a4d7849bdf67c0a2cd326cbd96aad6393a72992dc1faf61828074b29c4a867350d8d1"
    "ffee1ae2dda261bd10c35f679f29e4d370e5673ad22000cc251596544585ed82883b9f3bc3"
    "04d423b10ddcc8269d28b3254d52e533f3ed2cb81acfc352b42fc77996147d72277285ea6d"
    "41a022136d0683a4dd0f69d201cdc6b8645c2c79d1c7d331db2cffdad06931ad835fed6a97"
    "e40043b02e97ae005f5cb9e8398010ca0cfaf0b5cdaa271160d9218693919f2d1a8ede0bb5"
    "cb052430454d1175fde5a0a94e3a8c3b525a3718054a7a096ae6d5a9a671474c50e13e8a21"
    "2b4f0ee3cb72c5283e5a33ec48922ea12457090f01853b34397ec79062e2dc5d0a2c512695"
    "3a9592a5398f0c830b9d38ab982ac401c40d7713cbcaf128315275272cf00486c8f33df29d"
    "8f5552403faa227fe7693bee4409deffb0693aae74e99d33ae8b6d6004ff533f88e9639bb1"
    "6d2c22155a15d9e5cb03783cca598cc8c286ffd279d6c6ec5bbba0ae0120092e385dda5de0"
    "594ee58b848fb6e0569f21a1cfb20f2c93f8cf37c19f3298216552666ed3719855b9469f1a"
    "35c4476962704b779ee421e6325a2605ba5753d79b553cbb5379609cc84df7f51d54029168"
    "0eaaca5a780c289ac3ac49c0f485ee59767e284ef15c63f7ce0e2c21a058e901fdebd1afe6"
    "ef93b3955160a2744015e5f40aca6d9a37424d5a58490fe902fc77d859deddad4b992e6473"
    "ad422ff32c0d49e42e6ca47375181485bb64b4a1b06e01c0cf179cc528c32d6c172a3d065c"
    "f3b44975ad1769d4ca65ae4471a5f60d0f8e37c743ce6b08e9d134488fc9fcf35d2dec62d3"
    "f0b3fe2e40557654c7b46116cc7c1c1924e64dd4c377671f3c7479a1f885881d6fa47e2c21"
    "9f49f5aa4ef34afa9dbef6cedab5ab39bd1641a94aac0901ca";

const char* kOutput =
    "54306a13da596b6d5949c8c5ab26d48aadc03daf14b915b8cadf17a703d3c50601ef21dda3"
    "0b9e48b85e0b879f9523688569d25daf57e927113d49faf108cc15ec1d1916129bc8661ffa"
    "2c93f4991127310ed8464740117001cae85bc591c83adcaaf34b80e5bc03d08972bcce2a76"
    "0cf5da4c10063541b1e6b4aa7aeff0624ac59f2cafb82fd9d1017a362f3e83a5eb8170a057"
    "1746ea9ecb0e74d344571d4006f8b7cb5ff479bd1119d6eef8b0aadd0062ad3b889a885b1b"
    "07c9ae9ea694e555db4523b92ccd29d354c3881e5f52f2090026261aedf5c2a97df9215aaf"
    "6dab8e168496b54fcf1ea3af089f7986c3be0c70cb8ff3c5f8e84b217d18a9ed8bfb6b5a6f"
    "260b56047cfe0e1ec13f82c573bd530cf0e2c9f33d1b6dba70c16db60028e1c4786204da23"
    "86c3da743d7cd67629b2272eb235426082cf302c59e4e3d0741f58e8da4745731c0593ae75"
    "be1f81d8b7b3fffc8b529eed8b379fe0b8a266e16ac51f1df0de3f3db028f3aa4e4d31b026"
    "792b080fe92f79b3c8dda789a8a81d590e4f1e931f707f4e7efeb8ca63e0a605ccd7de2a49"
    "31785c5f44b29b91991429630912dd02d97be9f51207d0e7e6e8dddaa473c48ebd7bb7bbcb"
    "832f43f61c50ae9b2e52801885a823527a6af74236ca915a3d2aa0357d70fc4c187c5772cf"
    "9b29d6d0b4d7e6897069225e45094d4987845f8a5fe415d3e372afb2309cc1ff8e6d2a769e"
    "08037ee0c3c297066b332b08e3d50bd832676110ed6bed50efd71c1be06da16419342fe4e8"
    "54bf840edf0e8bd8dd7796b854abf295590d0d0a156e01f224aba0d8df38ea97587688beaf"
    "45e3564f68e84be72b22189682892534d1dd08ea7e21ef575543f7faca1cde992e8bd8c3cf"
    "894dfc3b7d4ac999c431b67aaef849b246c1600575f33df2c984a4b98a872a875c0abc517d"
    "9af5c9242d5ee6c6e3cd7ee4af8a6c0004c8d7a5adfab2084a269b7cd0c613b1b9653f7030"
    "f9989d879957713eb1c324f0a6a2609d66d25faee39487ead1ea0d2a77ef31ccebf90cdc9c"
    "1280bbb08eab9a04cd4b954f7a0b537c16cc0eb17310ddaa769490d98b664131ed8c7d74c4"
    "33fac3438d10bc844d0e9532df17436dd25e12b9ed33d9976f4acdc3cd8134be7ea2d0a791"
    "5d90f65e4a250fcc24ebe1e4626c8f4536975dda202b86008c94a96a69b2e9bb828e4195b4"
    "b7f15552303948b32582a9102789b5e51fab723c7008cee661bf19c8902b29303eb84c33f0"
    "f0152eb725ca994b6f4b4150ee5699cf2ba4c47c5ca6d467045c5d5f269e0fe258684c30cd"
    "ef46db376fbbc480ca8a545d719d0ce8b82c109044a4883fbc153cd2ca0ec3e46eefb0cbfd"
    "617c27f225ea716df7499c8127f06133cf5568d373a4ed35652af23ecf9098546d956a0c9c"
    "240eb4b79b8d6e1cbceb171086da916d894cebf5508f40cf4a";

} // namespace

namespace boring_crypto {

// Run the test with the test vector at all lengths.
bool ChaChaUnitTests(void*) {
    BEGIN_TEST;
    size_t max = strlen(kKey) / 2;
    uint8_t key[max];
    FromHex(kKey, key, max);
    max = strlen(kNonce) / 2;
    uint8_t nonce[max];
    FromHex(kNonce, nonce, max);
    max = strlen(kInput) / 2;
    uint8_t in[max];
    FromHex(kInput, in, max);
    max = strlen(kOutput) / 2;
    uint8_t out[max];
    FromHex(kOutput, out, max);
    uint8_t buf[max];
    for (size_t len = 0; len <= sizeof(kInput); len++) {
        CRYPTO_chacha_20(buf, in, len, key, nonce, kCounter);
        EXPECT_BYTES_EQ(out, buf, len, "cipher stream mismatch");
        // Test the in-place version.
        memcpy(buf, in, len);
        CRYPTO_chacha_20(buf, buf, len, key, nonce, kCounter);
        EXPECT_BYTES_EQ(out, buf, len, "cipher stream mismatch");
    }
    END_TEST;
}

} // namespace boring_crypto
