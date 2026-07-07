# Ke hoach bao cao README.md - De tai Radar tam ngan

## 1. Pham vi khao sat

### 1.1. Repo chinh

- Repository: `https://github.com/phamhungcrab/IT4210`
- Thu muc project chinh: `radar_short_range_touchgfx`
- Muc tieu bao cao: viet README.md tieng Viet cho bai tap lon he nhung, bam khung cua thay nhung trinh bay chuyen nghiep hon.

### 1.2. File mau da doi chieu

- Repo mau: `https://github.com/neittien0110/ProjectSample`
- File mau: `README.HENHUNG.md`
- Khung bat buoc can giu:
  - `GIOI THIEU`
  - `TAC GIA`
  - `MOI TRUONG HOAT DONG`
  - `SO DO SCHEMATIC`
  - `TICH HOP HE THONG`
  - `DAC TA HAM`
  - `KET QUA`

### 1.3. Nguyen tac khi viet README sau nay

- Khong viet hinh thuc; moi phan can giai thich duoc cach he thong hoat dong.
- Khong tu bia so lieu do thuc nghiem, thong so chua co, ket qua demo chua xac minh.
- Phan thieu du lieu phai ghi ro `TODO` hoac placeholder.
- Cac anh/video can chen sau phai de placeholder ro rang.
- Giu van phong ky thuat, mach lac, de doc, phu hop bao cao GitHub.

## 2. Cau truc repo quan trong

### 2.1. Tong quan thu muc

- `radar_short_range_touchgfx/`
  - Project STM32CubeMX/STM32CubeIDE cho board STM32F429I-DISC1.
  - Chua file cau hinh `.ioc`, ma nguon HAL/Core, FreeRTOS, TouchGFX va cac file user module.

- `radar_short_range_touchgfx/Core/`
  - Ma nguon sinh boi CubeMX va cac doan user code trong `main.c`.
  - Chua khoi tao clock, GPIO, I2C3, USART1, TIM2, TIM3, TIM5, LTDC, DMA2D, FMC/SDRAM, FreeRTOS task.

- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/`
  - Cac module ung dung radar tu viet.
  - Day la nhom file nen duoc nhac trong bao cao vi the hien logic chinh cua san pham.

- `radar_short_range_touchgfx/TouchGFX/`
  - Cau hinh, assets va ma giao dien TouchGFX.
  - Co cac man Home, Scan, Settings, Info va assets radar.

- `radar_short_range_touchgfx/DesignAssets/`
  - Tai nguyen hinh anh thiet ke, chu yeu phuc vu giao dien.
  - Co the nhac ngan trong README neu can mo ta UI assets.

- `radar_short_range_touchgfx/temp/`
  - Tai lieu tam/tham khao noi bo.
  - Khong nen dua vao README nhu tai lieu chinh neu chua xac minh noi dung.

### 2.2. File cau hinh quan trong

- `radar_short_range_touchgfx/STM32F429I_DISCO_REV_D01.ioc`
  - Cau hinh MCU: `STM32F429ZITx`.
  - CubeMX version: `6.17.0`.
  - Firmware package: `STM32Cube FW_F4 V1.28.3`.
  - TouchGFX: `X-CUBE-TOUCHGFX 4.26.1`.
  - FreeRTOS CMSIS v2.
  - Clock he thong: SYSCLK/HCLK 180 MHz theo file `.ioc`.
  - LTDC: 240 x 320, RGB565.
  - TouchGFX double buffering, LTDC display interface, DMA2D accelerator.

- `radar_short_range_touchgfx/readme.md`
  - File README mac dinh cua template TouchGFX.
  - Chi nen tham khao thong tin template, khong du de lam bao cao bai tap lon.

## 3. Module phan cung chinh

### 3.1. Board dieu khien

- STM32F429I-DISC1 / STM32F429ZIT6.
- Co LCD tich hop, SDRAM, LTDC va TouchGFX.
- Han che thuc te: nhieu chan da bi LCD/SDRAM/SPI/I2C cua board chiem dung, nen viec chon chan ngoai vi can can than.

### 3.2. Cam bien khoang cach HC-SR04

- Vai tro: do khoang cach vat can bang song sieu am.
- Ket noi trong project:
  - `PG2 = HCSR04_TRIG`
  - `PG3 = HCSR04_ECHO`
- Cau hinh trong code:
  - Trigger 10 us.
  - Echo bat bang EXTI3 tren PG3, canh len va canh xuong.
  - Do thoi gian echo bang DWT cycle counter.
  - Cong thuc trong code: `distance_cm = echo_us / 58`.
- Ghi chu can dua vao README:
  - Thong so danh dinh theo yeu cau: tam do 2-400 cm, trigger 10 us, song 40 kHz, ideal accuracy khoang 3 mm.
  - Sai so thuc te phu thuoc be mat vat, goc phan xa, nhieu, nguon cap va rung co khi.
  - Echo cua module HC-SR04 thuong o muc 5 V, can chu y tuong thich logic 3.3 V voi STM32.

### 3.3. Servo MG90S

- Vai tro: quay cam bien HC-SR04 theo goc quet.
- Ket noi trong project:
  - `PB4 = SERVO_PWM (TIM3_CH1)`
- Cau hinh TIM3 theo `.ioc`:
  - Prescaler = 89.
  - Period = 19999.
  - Pulse ban dau = 1500.
  - Voi timer clock 90 MHz tren APB1 timer, cau hinh nay tao tick 1 us va PWM 50 Hz.
- Driver hien tai:
  - `Servo_SetAngle(angle_deg)` map goc 0-180 sang pulse `550-2450 us`.
  - `Servo_Stop()` dua servo ve goc center 90 do.
- Ghi chu can dua vao README:
  - MG90S co stall torque tham khao 1.8 kgcm @4.8 V, 2.2 kgcm @6 V, toc do xap xi 0.1 s/60 do.
  - Stall torque khong dong nghia kha nang mang tai lien tuc ben vung.
  - TODO can xac minh lai voi nhom: code `radar_app.c` con comment cu ve servo 360 continuous, trong khi driver hien tai la MG90S 180 do.

### 3.4. OLED SH1106 I2C

- Theo thong tin de bai: OLED SH1106 giao tiep I2C.
- Pin mapping uu tien:
  - `PA8 = I2C3_SCL`
  - `PC9 = I2C3_SDA`
- Trong `.ioc`: I2C3 duoc cau hinh 100 kHz, pull-up tren SCL/SDA.
- Trang thai code hien tai:
  - Chua thay driver SH1106/OLED rieng trong `STM32CubeIDE/Application/User`.
  - `RadarUiData_t` co truong `oled_connected`, nhung `radar_app.c` dang gan `oled_connected = 0`.
- Cach viet trong README:
  - Co the nhac OLED la thanh phan phan cung du kien/tich hop.
  - Phan driver OLED/hien thi OLED can de `TODO` neu chua co code hoac anh demo.
  - Can ghi dung gioi han: SH1106 controller ho tro toi da 132x64, module OLED pho bien hien thi 128x64.

### 3.5. Buzzer active va LED

- `PC4 = BUZZER`.
- `PG13 = LED3_SCAN`.
- `PG14 = LED4_ALERT`.
- Module `buzzer_led.c`:
  - LED scan nhap nhay khi radar quet.
  - LED alert bat khi co vat/gan nguy hiem.
  - Buzzer chi keu khi `near_warning`.
  - Chu ky toggle buzzer trong alert gan: 120 ms.

### 3.6. Nut bam USER

- `PA0/WKUP = B1_USER`.
- Trong `StartDefaultTask()`:
  - Bam ngan 30-800 ms: doi toc do quet `SLOW -> MED -> FAST`.
  - Giu lau >= 800 ms: doi che do quet `180 deg <-> 90 deg`.

### 3.7. UART debug

- `PA9 = USART1_TX`.
- `PA10 = USART1_RX`.
- `radar_debug.c` dung `HAL_UART_Transmit()` de in log debug.
- Bao cao co the nhac UART dung de quan sat trang thai do SR04, counter EXTI, echo time va data UI.

## 4. Module phan mem chinh

### 4.1. `Core/Src/main.c`

- Khoi tao HAL, clock, GPIO, CRC, I2C3, SPI5, FMC/SDRAM, LTDC, DMA2D, TIM5, TIM2, USART1, TIM3, TouchGFX.
- Tao cac task FreeRTOS:
  - `defaultTask`: xu ly nut B1_USER.
  - `GUI_Task`: task TouchGFX.
  - `radarTask`: chay `StartRadarTask()`.
- `HAL_GPIO_EXTI_Callback()` chuyen interrupt echo PG3 sang `HCSR04_GPIO_EXTI_Callback()`.
- `StartRadarTask()` goi `RadarApp_Init()`, hien dang goi `RadarApp_Start()` ngay khi khoi dong de force chay radar khi debug.

### 4.2. `hcsr04.c/h`

- Driver do khoang cach HC-SR04.
- Luong do:
  - Phat TRIG 10 us tren PG2.
  - Cho ECHO len/xuong qua EXTI3 tren PG3.
  - Dung DWT `CYCCNT` de tinh `echo_us`.
  - Timeout theo `HCSR04_TIMEOUT_MS = 25 ms`.
  - Tra khoang cach cm neu hop le.
- Diem nen nhac trong README:
  - Cach thiet ke khong polling echo lien tuc, giam blocking so voi cho echo bang vong lap dai.
  - Van co cho ket qua trong `RadarApp_MeasureDistance()` bang delay 1 ms trong task, nen thoi diem do co the bi anh huong boi lich FreeRTOS va rung servo.

### 4.3. `servo_mg90s.c/h`

- Driver PWM servo tren TIM3 CH1.
- `Servo_Init()` start PWM va dua servo ve center.
- `Servo_SetPulseUs()` gioi han pulse trong `SERVO_MIN_PULSE_US` den `SERVO_MAX_PULSE_US`.
- `Servo_SetAngle()` map goc 0-180 sang duty pulse.
- `Servo_GetLastAngle()` va `Servo_GetLastPulseUs()` phuc vu debug/UI.

### 4.4. `radar_config.h`

- File tap trung tham so he thong:
  - Nguong do: min 2 cm, max display 50 cm, detect <= 50 cm, near warning <= 5 cm.
  - HC-SR04: trigger 10 us, timeout 25 ms.
  - Servo: 0/90/180 do, pulse 550/1500/2450 us.
  - Scan mode: 90 do tu 45-135, 180 do tu 0-180.
  - Speed mode: slow/medium/fast voi step va delay khac nhau.
  - UI geometry: tam radar, ban kinh, kich thuoc target dot.

### 4.5. `radar_types.h`

- Dinh nghia:
  - `RadarSpeedMode_t`: SLOW, MED, FAST.
  - `RadarStatus_t`: SCAN, DETECT, ALERT.
  - `RadarUiData_t`: struct trung tam de truyen du lieu giua logic radar va UI.

### 4.6. `radar_ui_bridge.c/h`

- Vung du lieu chia se giua C task va TouchGFX C++.
- Dung critical section ngan bang `__disable_irq()` / `__enable_irq()` khi doc/ghi struct.
- Co logic giu lai cac bien dieu khien `radar_enabled`, `speed_mode`, `scan_mode_deg` khi task cap nhat sensor data, tranh UI vua set thi bi task ghi de bang gia tri cu.

### 4.7. `radar_app.c/h`

- Module dieu phoi ung dung radar:
  - Init bridge, servo, buzzer/LED, HC-SR04.
  - Doc trang thai control tu UI bridge.
  - Dieu khien servo toi goc hien tai.
  - Doi servo on dinh theo delay mode.
  - Do khoang cach.
  - Xac dinh `detected` va `near_warning`.
  - Cap nhat LED/buzzer.
  - Cap nhat `RadarUiData_t`.
  - Advance goc quet theo che do 90/180 va speed mode.
- Trang thai can ghi chu:
  - `RadarApp_Start()` bat radar.
  - `RadarApp_Stop()` tat radar va dua output ve safe state.
  - `RadarApp_NextSpeedMode()` phuc vu nut bam.
  - `RadarApp_ToggleScanMode()` phuc vu nut bam.

### 4.8. `buzzer_led.c/h`

- Dieu khien PC4/PG13/PG14.
- `Alert_Update(detected, near_warning)` gom logic canh bao.

### 4.9. `radar_debug.c/h`

- Wrapper printf qua USART1.
- Co the dung trong bao cao de giai thich cach debug va kiem tra echo/counter.

### 4.10. TouchGFX UI

- `TouchGFX/gui/src/screenhome_screen/ScreenHomeView.cpp`
  - Vao Home thi goi `RadarApp_Stop()`.

- `TouchGFX/gui/src/screenscan_screen/ScreenScanView.cpp`
  - Vao Scan thi goi `RadarApp_Start()`.
  - Doc `RadarUiData_t` de hien thi angle, distance, status.
  - Doi mau sweep xanh/do theo detect/alert.
  - Map angle + distance thanh toa do target dot tren radar UI.

- `TouchGFX/gui/src/screensettings_screen/ScreenSettingsView.cpp`
  - Cho chon speed SLOW/MED/FAST.
  - Cho chon scan mode 90/180 do.

- `TouchGFX/gui/src/screeninfo_screen/ScreenInfoView.cpp`
  - Hien mode, speed, range gan nhat, last angle, object count.

## 5. Nhung phan co the viet chac chan trong README

### 5.1. Gioi thieu de tai

- De tai la he thong radar tam ngan dung STM32F429I-DISC1.
- Cam bien HC-SR04 duoc dat tren servo de quet theo goc.
- Khoang cach va trang thai phat hien vat can duoc hien thi tren LCD TouchGFX.
- LED/buzzer dung de bao trang thai quet va canh bao vat the gan.

### 5.2. Tac gia

Co the viet day du theo thong tin nguoi dung cung cap:

| MSSV | Ho ten | Email |
| --- | --- | --- |
| 20235318 | Nguyen Minh Giang | giang.nm235318@sis.hust.edu.vn |
| 20235333 | Bui Trung Hoang | hoang.bt235333@sis.hust.edu.vn |
| 20235342 | Pham Ngoc Hung | hung.pn235342@sis.hust.edu.vn |
| 20235421 | Khuong Anh Tai | tai.ka235421@sis.hust.edu.vn |

TODO: neu can cot phan cong cong viec, can nhom cung cap thong tin thuc te.

### 5.3. Moi truong hoat dong

Co the viet chac:

- MCU/board: STM32F429I-DISC1 / STM32F429ZIT6.
- STM32CubeMX 6.17.0, STM32Cube FW_F4 V1.28.3.
- TouchGFX 4.26.1.
- FreeRTOS CMSIS v2.
- LCD 240 x 320 RGB565 tren board.
- Cac ngoai vi: GPIO, EXTI3, TIM3 PWM, TIM2 input capture configured, I2C3, USART1, LTDC, DMA2D, FMC SDRAM.

### 5.4. So do schematic / pin mapping

Co the viet bang pin mapping:

| STM32F429I-DISC1 | Tin hieu | Module |
| --- | --- | --- |
| PB4 | SERVO_PWM / TIM3_CH1 | Servo MG90S |
| PG2 | HCSR04_TRIG | HC-SR04 |
| PG3 | HCSR04_ECHO / EXTI3 | HC-SR04 |
| PC4 | BUZZER | Buzzer active |
| PG13 | LED3_SCAN | LED on-board |
| PG14 | LED4_ALERT | LED on-board |
| PA0/WKUP | B1_USER | Nut user |
| PA8 | I2C3_SCL | OLED SH1106 / touch IO bus |
| PC9 | I2C3_SDA | OLED SH1106 / touch IO bus |
| PA9 | USART1_TX | UART debug |
| PA10 | USART1_RX | UART debug |

Can co canh bao muc logic 5 V cua ECHO HC-SR04.

### 5.5. Tich hop he thong

Co the viet theo kien truc:

- Tang phan cung: STM32, HC-SR04, servo, LED, buzzer, LCD TouchGFX, OLED I2C neu xac nhan.
- Tang driver: `hcsr04`, `servo_mg90s`, `buzzer_led`, `radar_debug`.
- Tang ung dung: `radar_app`, `radar_ui_bridge`.
- Tang UI: TouchGFX Home/Scan/Settings/Info.
- RTOS: tach task UI, radar task va default task doc nut bam.

### 5.6. Dac ta ham

Co the dac ta cac ham chinh:

- `RadarApp_Init()`
- `RadarApp_TaskLoop()`
- `RadarApp_Start()`
- `RadarApp_Stop()`
- `RadarApp_SetSpeedMode()`
- `RadarApp_SetScanMode()`
- `HCSR04_StartMeasure()`
- `HCSR04_GPIO_EXTI_Callback()`
- `HCSR04_GetDistanceCm()`
- `Servo_SetAngle()`
- `Alert_Update()`
- `RadarUiBridge_SetData()`
- `RadarUiBridge_GetData()`

### 5.7. Gioi han va danh gia ky thuat

Co the viet chac theo yeu cau va code:

- HC-SR04 khong phai radar RF, ma la sieu am; "radar" trong de tai la mo hinh quet vat can.
- Ket qua phu thuoc nguon, goc vat, vat lieu phan xa, rung servo.
- Nguon yeu co the gay trang man, reset, servo rung, so do nhieu.
- Do ngay khi servo chua on dinh co the lam nhieu so do.
- STM32F429I-DISC1 han che chan trong do LCD/SDRAM tich hop.
- Tich hop TouchGFX + FreeRTOS + do sensor + dieu khien servo can quan ly thoi gian va tai CPU can than.

## 6. Nhung phan can placeholder/TODO

### 6.1. Ket qua do thuc nghiem

Repo chua co bang do so sanh khoang cach thuc te/HC-SR04. README nen de bang mau:

| Lan do | Khoang cach thuc te (cm) | Gia tri hien thi (cm) | Sai so (cm) | Ghi chu |
| --- | ---: | ---: | ---: | --- |
| 1 | TODO | TODO | TODO | TODO |
| 2 | TODO | TODO | TODO | TODO |
| 3 | TODO | TODO | TODO | TODO |

### 6.2. Anh va video minh hoa

Can de placeholder:

- `[Chen anh: so do khoi he thong]`
- `[Chen anh: so do noi day thuc te]`
- `[Chen anh: bo tri HC-SR04 tren servo MG90S]`
- `[Chen anh: giao dien man hinh Home]`
- `[Chen anh: giao dien man hinh Scan]`
- `[Chen anh: giao dien man hinh Settings]`
- `[Chen anh: giao dien man hinh Info]`
- `[Chen anh: ket qua canh bao khi vat can gan]`
- `[Chen video: demo san pham quet va phat hien vat can]`

### 6.3. OLED SH1106

Can TODO vi chua thay driver OLED trong source user:

- TODO: xac nhan OLED da duoc lap va hien thi noi dung nao.
- TODO: bo sung anh OLED neu co.
- TODO: neu OLED chua dung trong code, README nen ghi la module du kien/phan cung co ket noi nhung chua hoan thien driver.

### 6.4. Phan cong cong viec

Nguoi dung da cung cap danh sach thanh vien nhung chua cung cap phan cong. Can TODO:

- Nguyen Minh Giang: TODO
- Bui Trung Hoang: TODO
- Pham Ngoc Hung: TODO
- Khuong Anh Tai: TODO

### 6.5. Kiem chung servo

Can xac minh:

- Servo thuc te dang dung co phai MG90S 180 do hay co luc da dung servo 360.
- Comment trong `radar_app.c` con noi ve servo 360 continuous, nhung driver `servo_mg90s.c` la dieu khien servo goc 0-180.
- README cuoi nen mo ta theo phan cung thuc te va code hien tai, dong thoi co the ghi "da hieu chinh tu phien ban servo 360 cu" neu nhom xac nhan.

### 6.6. Cach build/flash

Repo co STM32CubeIDE, MDK-ARM, EWARM, GCC/TouchGFX, nhung can xac nhan workflow nhom dung khi nop:

- TODO: build bang STM32CubeIDE hay TouchGFX Designer.
- TODO: cach flash board: STM32CubeProgrammer, debug probe tich hop ST-LINK, hay TouchGFX Designer.
- TODO: co can huong dan simulator TouchGFX khong.

## 7. Dan y README.md de xuat

### 7.1. Tieu de va tom tat

- `# Radar tam ngan - STM32F429I-DISC1 + TouchGFX`
- Mo ta 3-5 cau:
  - He thong quet vat can tam ngan bang HC-SR04 gan tren servo.
  - Hien thi truc quan tren LCD TouchGFX.
  - Canh bao bang LED/buzzer khi vat can nam trong nguong gan.
  - Neu co OLED thi ghi vai tro OLED sau khi xac minh.

### 7.2. Muc luc

- Gioi thieu
- Tac gia
- Moi truong hoat dong
- So do schematic
- Nguyen ly hoat dong
- Tich hop he thong
- Kien truc phan mem
- Dac ta ham
- Ket qua
- Gioi han thuc te va loi thuong gap
- Kinh nghiem rut ra
- Huong phat trien
- Tai lieu tham khao

### 7.3. GIOI THIEU

- De bai / muc tieu san pham.
- Huong tiep can:
  - Quet theo goc bang servo.
  - Do khoang cach bang HC-SR04.
  - Xu ly nguong detect/alert.
  - Hien thi TouchGFX va canh bao ngoai vi.
- Tinh nang:
  - Quet 90/180 do.
  - 3 toc do SLOW/MED/FAST.
  - Hien thi angle, distance, status.
  - Phat hien vat can <= 50 cm.
  - Canh bao gan <= 5 cm.
  - Nut USER doi speed/mode.
- Placeholder anh tong quan.

### 7.4. TAC GIA

- Bang thanh vien.
- Bang phan cong cong viec voi TODO neu chua co.

### 7.5. MOI TRUONG HOAT DONG

- Bang BOM:
  - STM32F429I-DISC1.
  - HC-SR04.
  - MG90S.
  - OLED SH1106 I2C.
  - Buzzer active.
  - LED on-board.
  - LCD TouchGFX.
- Bang moi truong phan mem:
  - STM32CubeMX/CubeIDE.
  - STM32Cube FW_F4.
  - TouchGFX.
  - FreeRTOS.
- Thong so cau hinh quan trong:
  - Clock.
  - TIM3 PWM.
  - EXTI3.
  - I2C3.
  - USART1.
  - LTDC/DMA2D/FMC.

### 7.6. SO DO SCHEMATIC

- Placeholder so do khoi.
- Placeholder so do noi day.
- Bang pin mapping.
- Luu y nguon va muc logic:
  - Servo nen cap nguon on dinh, chung GND voi STM32.
  - Echo HC-SR04 5 V can mach chia ap/level shifting neu module xuat 5 V.
  - I2C can pull-up phu hop 3.3 V.

### 7.7. NGUYEN LY HOAT DONG

- Chu trinh quet:
  - Servo dat goc.
  - Doi servo on dinh.
  - Phat trigger 10 us.
  - Bat echo canh len/xuong bang EXTI.
  - Tinh distance.
  - Cap nhat trang thai detect/alert.
  - Cap nhat UI/LED/buzzer.
  - Tang/giam goc quet.
- Cong thuc khoang cach:
  - `distance_cm = echo_us / 58`.
- Giai thich scan 90 do va 180 do.

### 7.8. TICH HOP HE THONG

- So do tang:
  - Hardware.
  - HAL/driver.
  - Application logic.
  - UI bridge.
  - TouchGFX.
- FreeRTOS task:
  - `defaultTask`.
  - `GUI_Task`.
  - `radarTask`.
- Dong du lieu:
  - `radarTask -> RadarUiBridge -> TouchGFX`.
  - `TouchGFX/USER button -> RadarApp/RadarUiBridge`.

### 7.9. KIEN TRUC PHAN MEM

- Bang file/module/chuc nang:
  - `main.c`
  - `radar_app.c`
  - `hcsr04.c`
  - `servo_mg90s.c`
  - `buzzer_led.c`
  - `radar_ui_bridge.c`
  - `radar_debug.c`
  - `ScreenScanView.cpp`
  - `ScreenSettingsView.cpp`
  - `ScreenInfoView.cpp`
  - `ScreenHomeView.cpp`
- Mo ta struct `RadarUiData_t`.

### 7.10. DAC TA HAM

- Chon 10-12 ham quan trong, moi ham gom:
  - Muc dich.
  - Dau vao/dau ra.
  - Ghi chu ky thuat.
- Nen co code snippet ngan cho `RadarUiData_t` hoac prototype ham.

### 7.11. KET QUA

- Placeholder anh giao dien.
- Placeholder video demo.
- Bang ket qua tinh nang:
  - Servo quet.
  - Do khoang cach.
  - Hien thi LCD.
  - LED scan.
  - Alert LED/buzzer.
  - Nut USER.
  - OLED: TODO.
- Bang do thuc nghiem mau voi TODO.

### 7.12. DANH GIA THUC TE VA GIOI HAN

- HC-SR04:
  - Tam do danh dinh 2-400 cm.
  - Song 40 kHz.
  - Accuracy ly tuong ~3 mm.
  - Thuc te phu thuoc nhieu yeu to.
- MG90S:
  - Torque/toc do theo thong so tham khao.
  - Khong nen hieu stall torque la tai lien tuc.
- SH1106:
  - Controller 132x64, module hay hien thi 128x64.
- Cac loi thuc te:
  - Nguon yeu.
  - Echo 5 V.
  - Servo rung.
  - Han che chan board.
  - Tich hop TouchGFX + logic thoi gian thuc.

### 7.13. KHO KHAN VA KINH NGHIEM RUT RA

- Quan ly timing echo sieu am.
- Can doi refresh UI va logic do.
- Can tach du lieu UI qua bridge de tranh race/ghi de control state.
- Can cap nguon rieng/on dinh cho servo.
- Can test tung module truoc khi ghep TouchGFX.

### 7.14. HUONG PHAT TRIEN

- Bo loc so do: median/moving average/outlier rejection.
- Dung queue/mutex RTOS thay vi disable IRQ neu mo rong du lieu.
- Hoan thien OLED neu chua co.
- Luu log do va hien thi bieu do.
- Them cau hinh nguong alert tren UI.
- Cai thien co khi gay rung servo.
- Them mach bao ve/level shifting cho echo.

### 7.15. TAI LIEU THAM KHAO

- Datasheet/reference manual STM32F429.
- UM1670/UM1671 cho STM32F429I-DISC1, neu dung.
- Tai lieu TouchGFX/ST.
- Datasheet/tai lieu HC-SR04.
- Datasheet/tai lieu MG90S.
- Datasheet/tai lieu SH1106.
- Repo mau bao cao cua thay.

## 8. Danh sach file nguon can doc tiep khi viet README hoan chinh

### 8.1. Bat buoc doc lai/doi chieu khi viet

- `radar_short_range_touchgfx/STM32F429I_DISCO_REV_D01.ioc`
- `radar_short_range_touchgfx/Core/Src/main.c`
- `radar_short_range_touchgfx/Core/Inc/main.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_config.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_types.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_app.c`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_app.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/hcsr04.c`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/hcsr04.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/servo_mg90s.c`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/servo_mg90s.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/buzzer_led.c`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/buzzer_led.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_ui_bridge.c`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_ui_bridge.h`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_debug.c`
- `radar_short_range_touchgfx/STM32CubeIDE/Application/User/radar_debug.h`

### 8.2. Can doc khi viet phan UI

- `radar_short_range_touchgfx/TouchGFX/gui/src/screenhome_screen/ScreenHomeView.cpp`
- `radar_short_range_touchgfx/TouchGFX/gui/src/screenscan_screen/ScreenScanView.cpp`
- `radar_short_range_touchgfx/TouchGFX/gui/src/screensettings_screen/ScreenSettingsView.cpp`
- `radar_short_range_touchgfx/TouchGFX/gui/src/screeninfo_screen/ScreenInfoView.cpp`
- `radar_short_range_touchgfx/TouchGFX/assets/texts/texts.xml`
- `radar_short_range_touchgfx/TouchGFX/*.touchgfx`

### 8.3. Can doc neu muon bo sung build/flash

- `radar_short_range_touchgfx/STM32CubeIDE/`
- `radar_short_range_touchgfx/MDK-ARM/`
- `radar_short_range_touchgfx/EWARM/`
- `radar_short_range_touchgfx/gcc/`
- `radar_short_range_touchgfx/TouchGFX/config/`

## 9. Danh sach hinh anh/video can chen sau

- `[Chen anh: so do khoi he thong]`
- `[Chen anh: so do noi day thuc te]`
- `[Chen anh: bo tri HC-SR04 tren servo MG90S]`
- `[Chen anh: mach chia ap/level shifting cho chan Echo neu co]`
- `[Chen anh: giao dien man hinh Home]`
- `[Chen anh: giao dien man hinh Scan khi dang quet]`
- `[Chen anh: giao dien man hinh Scan khi phat hien vat can]`
- `[Chen anh: giao dien man hinh Settings]`
- `[Chen anh: giao dien man hinh Info]`
- `[Chen anh: OLED SH1106 hien thi thong tin neu da hoan thien]`
- `[Chen anh: log UART debug tren terminal]`
- `[Chen video: demo san pham quet 180 do]`
- `[Chen video: demo doi mode 90/180 do va doi speed]`
- `[Chen video: demo canh bao vat can gan bang LED/buzzer]`

## 10. Thong tin con thieu can hoi nguoi dung hoac de TODO

- Phan cong cong viec cua tung thanh vien.
- Anh chup thuc te he thong va giao dien.
- Video demo san pham.
- Bang so lieu do thuc nghiem.
- Xac nhan OLED SH1106 da duoc tich hop bang code hay moi dung o muc phan cung.
- Xac nhan servo thuc te la MG90S 180 do, va co can nhac comment servo 360 cu trong README khong.
- Xac nhan workflow build/flash nhom muon dua vao README.
- Xac nhan co dung mach chia ap/level shifter cho Echo HC-SR04 hay khong.
- Xac nhan nguon cap servo/buzzer: lay tu board, nguon ngoai 5 V, hay cach cap khac.
- Xac nhan nguong detect 50 cm va near warning 5 cm co dung voi demo nop bai hay can dieu chinh.
