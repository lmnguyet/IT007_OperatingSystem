#include <stdio.h>

#define MAX 100

// seq[n]: chuoi tham chieu n trang
// frame_num: so luong khung trang do nguoi dung nhap
// available: bien danh dau trang co trong khung trang hay khong
// replace_i: bien luu vi tri khung trang se thay the, [0, frame_num-1], khoi tao = 0
// fault_num: so luong loi trang
// faults[n]: mang danh dau loi xay ra khi tham chieu trang nao
// total_frames[frame_num][n]: mang 2 chieu voi cot la cac khung trang tai thoi diem tham chieu 1 trang, 
// hang la khung trang tai vi tri nhat dinh tai nhieu thoi diem tham chieu trang khac nhau

void FIFO(int seq[], int n, int frame_num) {
    int i, j, available, replace_i=0, fault_num=0, faults[MAX];
    
    int total_frames[frame_num][n]; 
    for(j=0; j<n; j++) {
        printf("%d ", seq[j]); // in chuoi tham chieu ban dau
        faults[j]=0; // khoi tao ban dau khong trang nao co loi
    }
    printf("\n");

    for (i=0; i<frame_num; i++) {
        for (j=0; j<n; j++) total_frames[i][j]=-1; 
        // khoi tao ban dau tat ca cac o trong bang = -1 (toan bo khung trang rong)
    }

    for(j=0; j<n; j++) {
        available=0; // moi lan tham chieu 1 trang, xem nhu chua tim thay trong khung trang
        // bat dau tim trong khung trang
        for(i=0; i<frame_num; i++) {
            if(seq[j]==total_frames[i][j]) available=1; // tim thay trang
        }

        if(available==0) { // khong tim thay -> thay trang
            total_frames[replace_i][j]=seq[j]; 
            replace_i=(replace_i+1)%frame_num;
            faults[j]=1; // danh dau buoc nay co loi
            fault_num++; // tang so loi
        }

        if(j<n-1) { // copy khung trang tai thoi diem hien tai cho trang tiep theo
            for(i=0; i<frame_num; i++) 
                total_frames[i][j+1]=total_frames[i][j]; 
        }
    }
    
    // Xuat output
    // In bang khung trang
    for (i=0; i<frame_num; i++) {
        for(j=0; j<n; j++) {
            if(total_frames[i][j]!=-1) 
                printf("%d ", total_frames[i][j]);
            else printf("  ");
        }
        printf("\n");
    }
    // In loi va so loi
    for(j=0; j<n; j++) {
        if(faults[j]==1) 
            printf("* "); // in loi
        else printf("  ");
    }
    printf("\nNumber of Page Fault: %d", fault_num);
}

// options[frame_num]: mang lua chon thay the trang, 
// luu vi tri trong chuoi tham chieu cua cac trang tuong ung trong khung trang hien tai

void OPT(int seq[], int n, int frame_num) {
    int i, j, k, available, replace_i=0, fault_num=0, faults[MAX];
    int total_frames[frame_num][n];

    int options[frame_num];

    for(j=0; j<n; j++) {
        printf("%d ", seq[j]);
        faults[j]=0;
    }
    printf("\n");

    for (i=0; i<frame_num; i++) {
        for (j=0; j<n; j++) total_frames[i][j]=-1;
        options[i]=n; // gia tri khoi tao = n (trang khong duoc tham chieu trong tuong lai) 
    }

    for(j=0; j<n; j++) {
        available=0;

        for(i=0; i<frame_num; i++) {
            if(seq[j]==total_frames[i][j]) available=1;
        }

        if(available==0) { 
            if(replace_i<frame_num) // truong hop con trong khung trang thi chi can them vao
                total_frames[replace_i++][j]=seq[j];
            else { // truong hop khong con trong khung trang -> tim trang tham chieu xa nhat trong tuong lai
                for(i=0; i<frame_num; i++) {
                    for(k=j+1; k<n; k++) 
                        if(total_frames[i][j]==seq[k]) {
                            options[i]=k; 
                            break; // dung khi tim thay vi tri dau tien (gan nhat)
                        }
                }

                k=0; // bien trung gian de tim vi tri xa nhat (gia tri max cua options)
                for(i=0; i<frame_num; i++) {
                    if(options[i]>k) {
                        k=options[i]; 
                        replace_i=i; // replace_i luu vi tri khung trang chon de thay = index của options
                    }
                }

                for(i=0; i<frame_num; i++) options[i]=n; // reset options cho trang tiep theo

                total_frames[replace_i][j]=seq[j]; // thay trang vao vi tri replace_i

                replace_i=frame_num; // reset replace_i tranh nham truong hop khung trang con trong
            }
            // danh dau va dem loi
            faults[j]=1;
            fault_num++;
        }
        // copy khung trang cho trang tiep theo
        if(j<n-1) {
            for(i=0; i<frame_num; i++) 
                total_frames[i][j+1]=total_frames[i][j];
        }
    }
    // Xuat output
    for (i=0; i<frame_num; i++) {
        for(j=0; j<n; j++) {
            if(total_frames[i][j]!=-1) 
                printf("%d ", total_frames[i][j]);
            else printf("  ");
        }
        printf("\n");
    }

    for(j=0; j<n; j++) {
        if(faults[j]==1) 
            printf("* ");
        else printf("  ");
    }
    printf("\nNumber of Page Fault: %d", fault_num);
}

// LRU lam tuong tu OPT nhung voi chuoi tham chieu truoc vi tri hien tai
void LRU(int seq[], int n, int frame_num) {
    int i, j, k, available, replace_i=0, fault_num=0, faults[MAX];
    int total_frames[frame_num][n];

    int options[frame_num];

    for(j=0; j<n; j++) {
        printf("%d ", seq[j]);
        faults[j]=0;
    }
    printf("\n");

    for (i=0; i<frame_num; i++) {
        for (j=0; j<n; j++) total_frames[i][j]=-1;
        options[i]=-1;
    }

    for(j=0; j<n; j++) {
        available=0;
        for(i=0; i<frame_num; i++) {
            if(seq[j]==total_frames[i][j]) available=1;
        }
        if(available==0) {
            if(replace_i<frame_num) 
                total_frames[replace_i++][j]=seq[j];
            else {
                for(i=0; i<frame_num; i++) {
                    for(k=j-1; k>=0; k--) 
                        if(total_frames[i][j]==seq[k]) {
                            options[i]=k;
                            break;
                        }
                }
                
                k=n-1;
                for(i=0; i<frame_num; i++) {
                    if(options[i]<k) {
                        k=options[i]; 
                        replace_i=i;
                    }
                }
                for(i=0; i<frame_num; i++) options[i]=-1;
                total_frames[replace_i][j]=seq[j];
                replace_i=frame_num;
            }
            faults[j]=1;
            fault_num++;
        }
        if(j<n-1) {
            for(i=0; i<frame_num; i++) 
                total_frames[i][j+1]=total_frames[i][j];
        }
    }
    
    for (i=0; i<frame_num; i++) {
        for(j=0; j<n; j++) {
            if(total_frames[i][j]!=-1) 
                printf("%d ", total_frames[i][j]);
            else printf("  ");
        }
        printf("\n");
    }

    for(j=0; j<n; j++) {
        if(faults[j]==1) 
            printf("* ");
        else printf("  ");
    }
    printf("\nNumber of Page Fault: %d", fault_num);
}

int main()
{
    int def_seq[11] = {2, 1, 5, 2, 1, 2, 1, 1, 0, 0, 7}; // default referenced sequence
    int seq[MAX], n, i;

    int frame_num;

    int choice_seq;

    printf("--- Page Replacement algorithm ---");
    printf("\n1. Default referenced sequence");
    printf("\n2. Manual input sequence");
    
    do
    {
        printf("\n-------------------------");
        printf("\nYour choice: ");
        scanf("%d", &choice_seq);
    } while(choice_seq!=1 && choice_seq!=2);

    if(choice_seq==2)
    {
        printf("\nEnter the size of your input sequence: ");
        scanf("%d", &n);
        printf("\nEnter your input sequence: ");
        for (i=0; i<n; i++)
            scanf("%d", &seq[i]);
    }
    else
    {
        n=11;
        for (i=0; i<n; i++)
            seq[i]=def_seq[i];
    }

    printf("\n--- Page Replacement algorithm ---");
    printf("\nInput page frames: "); scanf("%d", &frame_num);
    printf("\n--- Select algorithm ---");
    printf("\n1. FIFO algorithm");
    printf("\n2. OPT algorithm");
    printf("\n3. LRU algorithm");

    int choice_algo;
    do
    {
        printf("\n-------------------------");
        printf("\nYour choice: ");
        scanf("%d", &choice_algo);
    } while(choice_algo<1 && choice_algo>3);

    printf("\n--- Page Replacement algorithm---\n");
    switch (choice_algo) {
        case 1:
            FIFO(seq, n, frame_num);
            break;
        case 2:
            OPT(seq, n, frame_num);
            break;
        case 3:
            LRU(seq, n, frame_num);
            break;
	}
    return 0;
}