# SoalShiftSISOP20_modul2_F10

### 1. Soal Satu :

Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:

Jawab : 

Fungsi untuk mengecek apakah string yang diinputkan * atau tidak
```Javascript
int isStar(char * str){
	int length = strlen(str);
	if(length == 1){
		if(strcmp(str, "*") == 0)return 1;
	}
	return 0;
}
```
- Panjang dari string yang diinputkan harus 1 (berupa *), sehingga menggunakan ```strlen()``` untuk menghitung panjang string
- Mengecek apakah string yang diinputkan * atau tidak meggunakan ```strcmp()```

Fungsi untuk mengecek apakah string yang diinputkan angka atau tidak
```Javascript
int isNumber(char * str){
	int length = strlen(str);
	for(int i = 0; i < length; i++){
		if(!isdigit(str[i]))
			return 0;
	}
    return 1;
}
```
- Mengetahui panjang dari string yang diinputkan menggunakan ```strlen``` 
- Mengecek apakah setiap angka yang diinputkan berupa digit desimal (0 - 9) atau tidak. Apabila bukan digit desimal, maka tidak dijalankan

Fungsi untuk nge print error
```Javascript
void printError(){
	printf("Invalid Parameter\n");
}
```

Fungsi untuk menetapkan parameter (berupa digit angka dan bintang)
```Javascript
void assignParam(int * num, int min, int max, char * str){
	if(isNumber(str)){
		*num = atoi(str);
		if(*num < min || *num > max){
			printError();
			exit(EXIT_FAILURE);
		}
	}else if(isStar(str)){
		*num = -1;
	}else{
		printError();
		exit(EXIT_FAILURE);
	}
}
```
- Kalau string yang diinputkan berupa angka (di cek menggunakan fungsi ```isNumber```), maka convert string ke integer menggunakan atoi.
- Lalu di cek apakah integer (second, minute, hour) yang diinput lebih dari batas (second min 0, max59; minute min 0, max 59; hour min 0, max 23 ). Apabila melebihi batas, maka ```printError```
- Kalau string yang diinputkan berupa angka (di cek menggunakan fungsi ```isStar```), maka tandai integer jadi -1
- Apabila karakter yang dimasukkan bukan digit angka atau *, maka ```printError```

Main Function
```Javascript
int main(int argc, char ** argv) {
	if(argc != 5){
		printError();
		exit(EXIT_FAILURE);
	}

	char * basharg[] = {"bash", argv[4], NULL};

	int second, minute, hour;
	
 	assignParanm(&second, 0, 59, argv[1]);
 	assignParanm(&minute, 0, 59, argv[2]);
 	assignParanm(&hour, 0, 23, argv[3]);

	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}

	FILE * file = fopen(argv[4], "r");
	if(!file){
		printf("Invalid Path\n");
		exit(EXIT_FAILURE);
	}
	pid_t pid, sid;

	pid = fork();

 	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
```
- Apabila jumlah argumen bukan 5, maka program error, karena argumen 0 tempat program dijalankan, argumen 1 untuk second, argumen 2 untuk minute, argumen 3 untuk hour, dan argumen 4 untuk menjalankan program
- ```char * basharg[]``` untuk menjalankan program
- Masukkan parameter detik, menit, dan jam melalui fungsi  ```assignParam```
- Mengubah working directory ke root menggunakan ```chdir```
- ```FILE * file = fopen(argv[4], "r");``` untuk mengecek apakah program .sh dapat di read / tidak, kalau tidak bs di baca error
- Selanjutnya melakukan langkah-langkah pembuatan proses daemon

```Javascript
while (1) {
		time_t t = time(NULL);
		struct tm * timeNow = localtime(&t);
		int timeToSleep = 0;
		if(hour == -1){

		}else if(timeNow->tm_hour <= hour){
			timeToSleep += hour - timeNow->tm_hour;
		}else{
			timeToSleep += 23 - timeNow->tm_hour + hour;
		}
		timeToSleep *= 60;
		if(minute == -1){

		}else if(timeNow->tm_min <= minute){
			timeToSleep += minute - timeNow->tm_min;
		}else{
			timeToSleep += 59 - timeNow->tm_min + minute;
		}
		timeToSleep *= 60;
		if(second == -1){
			
		}else if(timeNow->tm_sec <= second){
			timeToSleep += second - timeNow->tm_sec;
		}else{
			timeToSleep += 59 - timeNow->tm_sec + second;
		}
		if(timeToSleep == 0)timeToSleep = 1;
		sleep(timeToSleep);
		pid_t pid = fork();
		if(pid < 0)
			exit(EXIT_FAILURE);
		if(pid == 0){
			char dir[100];
			strcpy(dir, argv[4]);
			int dirEndIndex = 0;
			for(int i = strlen(dir); i >= 0; i--){
				if(dir[i] == '/'){
					dirEndIndex = i;
					break;
				}
			}
			dir[dirEndIndex] = '\0';
			chdir(dir);
			execv("/bin/bash", basharg);
		}
```
- Mengecek jam, apabila berupa *, maka dijalankan setiap jam (tidak pengaruh pada delay), jadi tidak menjalankan apa-apa. 
Namun apabila jam berupa digit desimal, maka ada 2 kemungkinan, yaitu digit input lebih dari / kurang dari waktu sekarang 
Apabila digit input kurang dari waktu sekarang, maka delaynya adalah 23 dikurang jam yang sekarang dan di tambah jam selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang jam 15.00, dan mau di eksekusi jam 00.00, maka 24 - 15 + 0 = 9 jam delay.
Apabila digit input lebih dari waktu sekarang, maka delaynya jam yang sekarang dan di kurang jam selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang jam 15.00, dan mau di eksekusi jam 18.00, maka 18 - 15 = 3 jam delay. Lalu timeToSleep nya di * 60 (di convert ke menit)
- Mengecek menit, apabila berupa *, maka dijalankan setiap menit. TimeToSleep yang sudah dijumlah pada saat mengecek jam akan dikurangkan dengan menit sekarang (karena perintah berjalan tiap menit apabila berupa bintang) sehingga tidak ada delay. 
Namun apabila menit berupa digit desimal, maka ada 2 kemungkinan, yaitu digit input lebih dari / kurang dari waktu sekarang 
Apabila digit input kurang dari waktu sekarang, maka delaynya adalah 59 dikurang menit yang sekarang dan di tambah menit selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang menit ke 45, dan mau di eksekusi pada menit ke 30, maka 60 - 45 + 30 = 45 menit delay. * 60 (di convert ke menit)
Apabila digit input lebih dari waktu sekarang, maka delaynya menit yang sekarang dan di kurang menit selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang menit ke 45 dan mau di eksekusi di menit ke 50 , maka 50 - 45 = 5 menit delay.
- Mengecek detik, apabila berupa *, maka dijalankan setiap detik.  TimeToSleep yang sudah dijumlah pada saat mengecek menit akan dikurangkan dengan detik sekarang (karena perintah berjalan tiap detik apabila berupa bintang sehingga tidak ada delay).
Namun apabila detik berupa digit desimal, maka ada 2 kemungkinan, yaitu digit input lebih dari / kurang dari waktu sekarang 
Apabila digit input kurang dari waktu sekarang, maka delaynya adalah 59 dikurang detik yang sekarang dan di tambah detik selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang detik ke 45, dan mau di eksekusi pada detik ke 30, maka 60 - 45 + 30 = 45 detik delay.
Apabila digit input lebih dari waktu sekarang, maka delaynya detik yang sekarang dan di kurang detik selanjutnya dimana perintah mau dieksekusi. 
Misalkan ingin sekarang detik ke 45 dan mau di eksekusi di detik ke 50 , maka 50 - 45 = 5 detik delay.
- ```if(timeToSleep == 0)timeToSleep = 1``` digunakan untuk mengset nilai minimum delay yaitu 1 detik
- Buar child process menggunakan ```fork```
- Mengubah directory menggunakan ```chdir()``` agar hasil yang diperoleh akan masuk ke directory yang kita mau
- Eksekusi perintah menggunakan ```execv```

### 2. Soal Dua :

#### a.Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss].
Jawab :
```Javascript
		pid_t p = fork();
		time_t t = time(NULL);
		struct tm tt = *localtime(&t);
		char folderName[20];
		snprintf(folderName, sizeof(folderName), "%04d-%02d-%02d_%02d:%02d:%02d", tt.tm_year + 1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
		if(p == 0){
			p = fork();
			if(p == 0){
				char * arg[] = {"mkdir", "-p", folderName, NULL};
				execv("/bin/mkdir", arg);
			}
			wait(NULL)
```
- Apabila child process, maka buat direktori menggunakan command ```mkdir -p``` dengan nama yang sudah di tentukan. 
- Memberi nama pada direktori menggunakan  ```struct tm tt = *localtime(&t)``` untuk mengambil argument tipe data ```time_t t``` dimana t adalah detik Epoch UNIX yang mengambil waktu [YYYY-mm-dd_HH:ii:ss]
-  Jalankan program menggunakan perintah ```execv()```
- Menggunakan ```wait(NULL)``` untuk mendelay proses selanjutny sampai child process (membuat direktori) selesai
- Ada perintah ```sleep(30)``` pada bagian paling akhir dr kodingan yang menunjukkan akan dilakukan perulangan terus setiap 30 detik sampai daemon process nya di kill

#### b.Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel  dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].
Jawab :
```Javascript
			 for(int i = 0; i < 20; i++){
				p = fork();
				if(p == 0){
					t = time(NULL);
					tt = *localtime(&t);
					char name[50];
					snprintf(name, sizeof(name), "./%s/%04d-%02d-%02d_%02d:%02d:%02d", folderName, tt.tm_year + 1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
					char url[25];
					snprintf(url, sizeof(url), "https://picsum.photos/%ld", t%1000+100);
					char * arg[] = {"wget", "-O", name, url, NULL};
					execv("/usr/bin/wget", arg);
				}
				wait(NULL);
				sleep(5);
			}
```
- Perulangan sebanyak 20 kali untuk menyimpan gambar, dan di akhir perulangan menggunakan perintah ```wait(NULL) untuk mendelay proses berikutnya (zip folder) dan ```sleep(5)``` untuk mendownload gambar setiap 5 detik
- Melakukan ```fork()``` agar terbentuk child process, lalu memakai perintah ```snprintf()``` memformat dan menyimpan nama gambar yang di ber format [YYYY-mm-dd_HH:ii:ss] menggunakan  ```struct tm tt = *localtime(&t)``` untuk mengambil argument tipe data ```time_t t``` dimana t adalah detik Epoch UNIX yang mau disimpan di nama (nama dari gambar). Begitu juga dengan untuk mendownload gambar, https://picsum.photos/%ld harus di format dan disimpan dulu di url menggunakan ```snprintf()``` sebelum menggunakan perintah ```wget()``` untuk mendownload gambar
- Menggunakan perintah ```wget -O``` untuk mendownload gambar dari url dan menyimpan di file 'nama'
- Eksekusi perintah menggunakan ```execv()```

#### c.Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).
Jawab :
```Javascript
			p = fork();
			if(p == 0){
				char fileName[30];
				snprintf(fileName, sizeof(fileName), "%s.zip", folderName);
				char * arg[] = {"zip", fileName, folderName, NULL};
				execv("/usr/bin/zip", arg);
			}
			wait(NULL);
			char * arg[] = {"rm", "-r", folderName};
			execv("/bin/rm", arg);
			break;
		}
```
- Melakukan ```fork()``` agar terbentuk child process dan menggunakan perintah ```snprintf()``` memformat dan menyimpan nama dari zip ke array fileName
- Men-zip file menggunakan perintah ```zip``` dan dieksekusi menggunakan perintah ```execv()```
- Lalu direktori yang di zip di hapus menggunakan perintah ```rm -r``` dan dieksekusi menggunakan ```execv()```

#### d.Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut.  Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.
Jawab :
```Javascript
	FILE * killer = fopen("killer.sh", "w");
	fprintf(killer, "#!/bin/bash\n");
	if(strcmp("-a", argv[1]) == 0){
		fprintf(killer, "kill $(pidof soal2)\n");
	}else if(strcmp("-b", argv[1]) == 0){
		fprintf(killer, "kill_parent(){\n");
		fprintf(killer, "kill ${@: -1}\n");
		fprintf(killer, "}\n");
		fprintf(killer, "kill_parent $(pidof soal2)\n");
	}
	fprintf(killer, "rm $0\n");
	fclose(killer);

	chmod("killer.sh", ~0);

```
- Untuk membuat file exececutable, dibuat file shell script. Dengan menggunakan ```fopen()``` dengan ekstensi .sh
- Memberikan permisi ke linux untuk ngerun menggunakan fungsi ```chmod()```
- Untuk membuat script menghapus diri sendiri, dapat dipakai fungsi ```rm $0```. Dimana ```$0``` adalah nama file yang sedang berjalan ini. 

#### e.Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).
Jawab :
```Javascript
	if(strcmp("-a", argv[1]) == 0){
		fprintf(killer, "kill $(pidof soal2)\n");
	}else if(strcmp("-b", argv[1]) == 0){
		fprintf(killer, "kill_parent(){\n");
		fprintf(killer, "kill ${@: -1}\n");
		fprintf(killer, "}\n");
		fprintf(killer, "kill_parent $(pidof soal2)\n");
	}
```
- Untuk menghentikan prosesnya, dapat menggunakan output ```pidof``` dimana akan berisi semua pid dari semua proses. Dengan parent process-nya di yang paling akhir.
- Jika argumennya ```-a``` maka akan menghentikan semua proses dengan nama soal2
- Jika argumennya ```-b``` maka akan menghentikan proses dengan pid paling kanan yang dapat diambil dari arg fungsi ```kill_parent``` dengan syntax ```${@: -1}```

### 3. Soal Tiga :

#### a. Program buatan jaya harus bisa membuat dua direktori di “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
Jawab : 
```Javascript
	pid_t pid = fork();

	if(pid < 0)
		exit(EXIT_FAILURE);

	if(chdir("/home/geizka") < 0){
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		pid = fork();
		if(pid < 0)
			exit(EXIT_FAILURE);
		if(pid == 0){
			char * arg[] = {"mkdir", "-p", "./modul2/indomie", NULL};
			execv("/bin/mkdir", arg);
		}
		wait(NULL);
		sleep(5);
		char * arg[] = {"mkdir", "-p", "./modul2/sedaap", NULL};
		execv("/bin/mkdir", arg);
	}
```
- Untuk membuat direktori pertama (indomie), ```fork()``` dulu. Apabila merupakan child process, maka buat direktori menggunakan ```mkdir -p```, lalu jalankan program menggunakan perintah ```execv()```
- ```wait(NULL)``` untuk mendelay proses pembuatan direktori sedaap yang merupakan proses selanjutnya
- Menggunakan perintah ```sleep(5)``` karena direktori sedaap akan dibuat 5 detik setelah direktori indomie dibuat 

#### b. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori“/home/[USER]/modul2/”. 
Jawab : 
```Javascript
pid = fork();
	if(pid < 0)
		exit(EXIT_FAILURE);
	if(pid == 0){
		char * arg[] = {"unzip", "./modul2/jpg.zip", "-d", "./modul2", NULL};
		execv("/usr/bin/unzip", arg);
	}
	wait(NULL);
```
 - Untuk meng unzip file, ```fork()``` dulu. Apabila merupakan child process, maka unzip jpg menggunakan ```unzip()``` , lalu jalankan program menggunakan perintah ```execv()```
 - Menggunakan system call ```wait()``` untuk mendelay proses pemindahan file / direktori ( proses soal no 3 )

#### c. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (didalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”.
Jawab :
```Javascript
	struct dirent * de;
	DIR * directory = opendir("./modul2/jpg");
	while((de = readdir(directory)) != NULL){
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;
		char fileName[300];
		char destName[300];
		if(de->d_type == DT_DIR){
			snprintf(fileName, sizeof(fileName), "./modul2/jpg/%s", de->d_name);
			snprintf(destName, sizeof(destName), "./modul2/indomie/%s", de->d_name);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"mv", fileName, destName, NULL};
				execv("/bin/mv", arg);
			}
			else{
			snprintf(fileName, sizeof(fileName), "./modul2/jpg/%s", de->d_name);
			snprintf(destName, sizeof(destName), "./modul2/sedaap/%s", de->d_name);
			pid = fork();
			if(pid == 0){
				char * arg[] = {"mv", fileName, destName, NULL};
				execv("/bin/mv", arg);
			}
			wait(NULL);
```
- ```wait(NULL)``` untuk mendelay proses pembuatan file coba2.txt yang merupakan rposes selanjutnya
-  ```struct dirent * de``` adalah struct untuk membaca file yang ada di direktori
- Perintah akan dijalankan apabila directory tidak kosong, sehingga menggunakan perintah ```readdir(directory)```
- Apabila direktori berupa . dan .., maka direktori tidak termasuk (proses jalan terus / continue), sehingga harus dibandingkan dengan de menggunakan ```strcmp()```
- Apabila tipe de merupakan direktori, maka menggunakan fungsi ```snprintf()``` memformat dan menyimpan nama hasil ekstrak (yang ber tipe direktori) yang mau disimpan di direktori indomie dalam buffer array.
- Memindahkan nama hasil ekstrak (yang ber tipe direktori) ke direktori indomie menggunakan ```mv```, setelah di ```fork()``` terlebih dahulu
- Apabila tipe de merupakan file, maka menggunakan fungsi ```snprintf()``` memformat dan menyimpan nama hasil ekstrak (yang ber tipe file) yang mau disimpan di direktori sedaap dalam buffer array.
- Memindahkan nama hasil ekstrak (yang ber tipe file) ke direktori sedaap menggunakan ```mv```, setelah di ```fork()``` terlebih dahulu

#### d. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu3 detik kemudian membuat file bernama “coba2.txt”. (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
Jawab : 
```Javascript
snprintf(fileName, sizeof(fileName), "%s/coba1.txt", destName);
pid = fork();
if(pid == 0){
	char * arg[] = {"touch", fileName, NULL};
	execv("/usr/bin/touch", arg);
}
wait(NULL);
sleep(3);
snprintf(fileName, sizeof(fileName), "%s/coba2.txt", destName);
pid = fork();
if(pid == 0){
	char * arg[] = {"touch", fileName, NULL};
	execv("/usr/bin/touch", arg);
}
wait(NULL);
```
-  Apabila de bertipe direktori, maka ada 2 file kosong yang akan dibuat, yaitu file coba1.txt dan coba2.txt.
- Menggunakan fungsi ```snprintf()``` memformat dan menyimpan coba1.txt yang mau disimpan di direktori dalam buffer array.
- Membuat file coba1.txt menggunakan perintah ```touch()```, dan di eksekusi menggunakan perintah ```execv()```
- ```wait(NULL)``` untuk mendelay proses pembuatan file coba2.txt yang merupakan rposes selanjutnya
- Menggunakan perintah ```sleep(3)``` karena file coba2.txt akan dibuat 3 detik setelah coba1.txt dibuat 
- Menggunakan fungsi ```snprintf()``` memformat dan menyimpan coba2.txt yang mau disimpan di direktori dalam buffer array.
- Membuat file coba2.txt menggunakan perintah ```touch()```, dan di eksekusi menggunakan perintah ```execv()```