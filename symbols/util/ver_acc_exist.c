int ver_acc_exist(struct spwd *acc_shadow_entry, char *acc_pass)
{
    if(!acc_shadow_entry || strlen(acc_shadow_entry->sp_pwdp) < 2) return 0;
    char *cpass;
    if((cpass = crypt(acc_pass, acc_shadow_entry->sp_pwdp)) == NULL) return 0;
    if(!strcmp(cpass, acc_shadow_entry->sp_pwdp)) return 1;
    return 0;
}
