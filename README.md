# Voxelusasi Objek 3D Menggunakan Octree

## Deskripsi Program

Program ini mengubah model 3D berbentuk mesh (file OBJ) menjadi representasi diskrit berupa voxel (kubus-kubus kecil) menggunakan struktur data octree. Setiap voxel merupakan bounding box berbentuk kubus yang menandai area di mana model 3D tersebut ada.

**Proses Kerja:**
1. Membaca berkas model 3D dalam format OBJ
2. Membagi ruang 3D menjadi grid hierarki menggunakan octree
3. Menentukan voxel yang berpotongan dengan permukaan model (segitiga)
4. Menyimpan hasil voxel sebagai berkas OBJ dan menampilkannya secara visual

**Aplikasi Praktis:**
- **Deteksi Tabrakan (Collision Detection)**: Memeriksa interaksi antar objek dengan lebih cepat
- **Indeks Spasial**: Pencarian cepat objek berdasarkan lokasi di ruang 3D
- **Rendering Volumetrik**: Menampilkan volume dan kepadatan objek
- **Permainan dan Simulasi Fisika**: Optimasi komputasi pada objek 3D yang kompleks

## Teori

### Voxelusasi

Voxelusasi adalah proses membagi model 3D menjadi unit-unit kubus kecil yang disebut voxel. Sebagai analogi, bayangkan model patung dibagi menjadi banyak kotak kecil berukuran sama. Selanjutnya, ditentukan kotak mana yang berpotongan dengan permukaan model.

Setiap unit voxel diberi status:
- **Kosong**: Tidak menyentuh objek model
- **Terisi**: Berisi bagian dari permukaan atau volume model

Hasil akhirnya adalah model 3D yang tidak lagi halus (smooth), melainkan terdiri dari kumpulan kubus-kubus diskrit.

### Struktur Octree

Octree adalah struktur hierarki untuk membagi ruang 3D secara efisien dalam proses voxelusasi. Prinsip kerjanya sebagai berikut:

1. **Level 1**: Ruang dibagi menjadi 8 subregion yang sama (struktur 2×2×2)
2. **Level 2**: Setiap subregion yang memotong model dibagi lagi menjadi 8 bagian lebih kecil
3. **Level 3**: Pembagian berlanjut sampai mencapai kedalaman maksimum yang ditentukan
4. **Optimasi**: Subregion yang kosong (tidak memotong model) tidak dibagi lebih lanjut

**Keuntungan:**
- **Efisiensi Memori**: Hanya menyimpan region yang berpotongan dengan model, menghemat ruang penyimpanan
- **Adaptif**: Area dengan geometri kompleks dibagi lebih banyak tingkat, area sederhana tidak perlu dibagi banyak
- **Akses Cepat**: Struktur hierarki memungkinkan pencarian dan akses data lebih cepat

## Persyaratan

### Perangkat Lunak
- **C++ Compiler**: C++17 atau lebih baru (clang++ atau g++)
- **Build Tool**: GNU Make
- **pkg-config**: Untuk pengelolaan perpustakaan
- **Raylib**: Perpustakaan grafis untuk visualisasi 3D (versi 4.0+)

### Dependensi Sistem

**Windows (MSYS2/MinGW)**:
```bash
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-raylib
pacman -S mingw-w64-x86_64-pkg-config
```

**Linux (Ubuntu/Debian)**:
```bash
sudo apt-get install build-essential pkg-config
sudo apt-get install libraylib-dev
```

**macOS**:
```bash
brew install raylib pkg-config
```

## Cara Kompilasi dan Menjalankan

### Kompilasi

Navigasi ke direktori `src` dan jalankan:

**Untuk clean build (direkomendasikan pada kali pertama atau saat ada masalah):**
```bash
cd src
make clean
make run
```

**Untuk build inkremental (setelah kompilasi pertama):**
```bash
cd src
make run
```

**Penjelasan:**
- `make clean` menghapus semua artifact kompilasi untuk memastikan pembangunan dari awal
- `make run` secara otomatis mengompilasi hanya berkas yang berubah, kemudian menjalankan program
- Jika tidak ada perubahan pada berkas sumber, `make run` langsung menjalankan executable yang telah ada

Berkas executable dihasilkan di `../bin/voxelizer`.

**Catatan untuk pengguna Windows:**
- Bisa menggunakan **MinGW/MSYS2** (lihat bagian Dependensi Sistem) untuk kompilasi langsung di Windows
- Alternatif lain adalah menggunakan **WSL (Windows Subsystem for Linux)** dan mengikuti instruksi Linux
- Tidak wajib menggunakan WSL, MinGW/MSYS2 sudah cukup untuk mengompilasi program ini

### Menjalankan Program

Dari direktori `src`:
```bash
./voxelizer
```

Atau:
```bash
make run
```

### Input

Ketika program dimulai, pengguna diminta untuk memasukkan:

1. **Path ke berkas .obj**: Masukkan path relatif atau absolut ke berkas model 3D dalam format OBJ
   - Contoh: `../test/model.obj`

2. **Kedalaman maksimum octree**: Masukkan level kedalaman maksimum untuk octree (integer >= 1)
   - Nilai lebih besar menghasilkan voxel lebih halus tetapi memerlukan komputasi lebih banyak
   - Rentang umum: 5-12

### Output

- **Model Voxelisasi**: Disimpan sebagai `../test/output.obj` berisi representasi voxel dalam bentuk mesh
- **Viewer 3D**: Jendela visualisasi interaktif terbuka secara otomatis untuk menampilkan model voxelisasi
- **Statistik**: Keluaran konsol menampilkan:
  - Jumlah vertex dan segitiga pada model masukan
  - Statistik octree (jumlah node per tingkat kedalaman, node yang dipangkas)
  - Waktu eksekusi (dalam milidetik)
  - Total jumlah voxel

## Struktur Proyek

```
.
├── src/
│   ├── main.cpp                    # Titik masuk program
│   ├── Makefile                    # Konfigurasi build
│   ├── model/                      # Struktur data
│   │   ├── Mesh.{cpp,hpp}         # Representasi mesh 3D
│   │   ├── Triangle.{cpp,hpp}     # Struktur data segitiga
│   │   ├── Vector.{cpp,hpp}       # Operasi vektor 3D
│   │   └── BoundingBox.{cpp,hpp}  # Definisi kotak pembatas
│   ├── parser/                     # Parser berkas
│   │   └── ObjParser.{cpp,hpp}    # Parser berkas OBJ
│   ├── octree/                     # Algoritma utama
│   │   └── Octree.{cpp,hpp}       # Pembangunan octree
│   ├── exporter/                   # Pengekspor keluaran
│   │   └── ObjExporter.{cpp,hpp}  # Ekspor ke format OBJ
│   ├── viewer/                     # Visualisasi
│   │   ├── Viewer.{cpp,hpp}       # Visualisasi dengan Raylib
│   │   └── fonts/                 # Aset font
│   └── stats/                      # Pengumpulan statistik
│       └── stats.{cpp,hpp}        # Metrik kinerja
├── bin/                            # Berkas biner terkompilasi (dihasilkan)
├── test/                           # Model uji dan keluaran
├── doc/                            # Dokumentasi
│   └── laporan.tex                # Laporan teknis
└── README.md                       # File ini
```

## Contoh Penggunaan

```
Path ke .obj file: ../test/model.obj
Max depth octree: 8

Mesh loaded: 100 vertices, 50 triangles

Execution time: 45.23 ms

--- Octree Statistics ---
Total voxels: 1024
Nodes per depth: [1, 8, 64, 512, 2048]
Pruned nodes per depth: [0, 2, 15, 120]

Jendela Viewer 3D terbuka...
```

## Keterangan Penting

- Program menggunakan multithreading selama pembangunan octree untuk meningkatkan kinerja
- Subregion kosong dipangkas untuk mengoptimalkan penggunaan memori
- Viewer dapat dinavigasi menggunakan kontrol mouse dan keyboard (lihat dokumentasi Raylib)
- Berkas format OBJ harus valid dengan definisi vertex dan face yang benar
