#ifndef COMD_H
#define COMD_H

class COMD {
public:
    SDS* comd;
    COMD next;
    SDS head;
    COMD();
    COMD(SDS comd,COMD *head);
    ~COMD();
    void run();
};

#endif