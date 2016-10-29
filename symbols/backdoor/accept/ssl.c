int gen_cert(X509 **cert, EVP_PKEY **key);
SSL_CTX *InitCTX(void);

SSL_CTX *InitCTX(void)
{
    #ifdef DEBUG
        printf("[vlany] InitCTX() called\n");
    #endif

    const SSL_METHOD *method;
    X509 *cert;
    EVP_PKEY *key;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = SSLv3_server_method();
    ctx = SSL_CTX_new(method);

    if(ctx == NULL)
    {
        abort();
    }

    SSL_CTX_set_options(ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2);

    char *ssl_cipher_list = strdup(SSL_CIPHER_LIST);
    xor(ssl_cipher_list);

    SSL_CTX_set_cipher_list(ctx, ssl_cipher_list);

    cleanup(ssl_cipher_list, strlen(ssl_cipher_list));

    if(gen_cert(&cert, &key) == 0 || SSL_CTX_use_certificate(ctx, cert) != 1 || SSL_CTX_use_PrivateKey(ctx, key) != 1)
    {
        exit(1);
    }

    X509_free(cert);
    EVP_PKEY_free(key);

    return ctx;
}

int gen_cert(X509 **cert, EVP_PKEY **key)
{
    #ifdef DEBUG
        printf("[vlany] gen_cert() called\n");
    #endif

    RSA *rsa;
    X509_NAME *subj;
    X509_EXTENSION *ext;
    X509V3_CTX ctx;

    char *common_name_host = strdup(COMMON_NAME_HOST); xor(common_name_host);
    const char *commonName = common_name_host;
    cleanup(common_name_host, strlen(common_name_host));

    char DNSName[128];
    int rc;

    *cert = NULL;
    *key = NULL;
    *key = EVP_PKEY_new();
    if(*key == NULL) exit(1);

    do {
        rsa = RSA_generate_key(DEFAULT_KEY_BITS, RSA_F4, NULL, NULL);
        if(rsa == NULL) exit(1);
        rc = RSA_check_key(rsa);
    } while(rc == 0);

    if(rc == -1) exit(1);

    if(EVP_PKEY_assign_RSA(*key, rsa) == 0)
    {
        RSA_free(rsa);
        exit(1);
    }

    *cert = X509_new();
    if(*cert == NULL || X509_set_version(*cert, 2) == 0) exit(1);

    subj = X509_get_subject_name(*cert);

    char *common_name = strdup(COMMON_NAME); xor(common_name);

    if(X509_NAME_add_entry_by_txt(subj, common_name, MBSTRING_ASC, (unsigned char *) commonName, -1, -1, 0) == 0)
    {
        cleanup(common_name, strlen(common_name));
        exit(1);
    }

    cleanup(common_name, strlen(common_name));

    char *dns = strdup(DNS); xor(dns);
    rc = snprintf(DNSName, sizeof(DNSName), "%s%s", dns, commonName);
    cleanup(dns, strlen(dns));
    if(rc < 0 || rc >= sizeof(DNSName)) exit(1);

    X509V3_set_ctx(&ctx, *cert, *cert, NULL, NULL, 0);

    char *subject_alt_name = strdup(SUBJECT_ALT_NAME); xor(subject_alt_name);

    ext = X509V3_EXT_conf(NULL, &ctx, subject_alt_name, DNSName);

    cleanup(subject_alt_name, strlen(subject_alt_name));

    if(ext == NULL || X509_add_ext(*cert, ext, -1) == 0) exit(1);

    char *certificate_comment = strdup(CERTIFICATE_COMMENT); xor(certificate_comment);
    char *ns_comment = strdup(NS_COMMENT); xor(ns_comment);

    ext = X509V3_EXT_conf(NULL, &ctx, ns_comment, certificate_comment);

    if(ext == NULL || X509_add_ext(*cert, ext, -1) == 0)
    {
        cleanup(certificate_comment, strlen(certificate_comment));
        exit(1);
    }

    cleanup(ns_comment, strlen(ns_comment));
    cleanup(certificate_comment, strlen(certificate_comment));

    X509_set_issuer_name(*cert, X509_get_subject_name(*cert));
    X509_gmtime_adj(X509_get_notBefore(*cert), 0);
    X509_gmtime_adj(X509_get_notAfter(*cert), DEFAULT_CERT_DURATION);
    X509_set_pubkey(*cert, *key);

    if(X509_sign(*cert, *key, EVP_sha1()) == 0) exit(1);

    return 1;
}
