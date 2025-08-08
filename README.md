# Inventory
[시연영상](https://youtu.be/K9rdjz1qtuw)

로스트아크의 인벤토리를 모작했습니다.  
## 기술 스택
- C++
- DirextX11
- FMOD

## 기능, 조작키
i : 인벤토리 열기  
1~9 : 아이템코드에 맞는 아이템 추가  
우클릭: 아이템 사용  
휠클릭: 아이템 정렬잠금/해제  
활성화된 버튼: 정렬, 펫슬롯  

아이템을 좌클릭으로 끌어서 다른 아이템 슬롯에 놓거나 아이템간의 위치를 바꿀 수 있습니다.  
펫슬롯 버튼으로 아이템을 끌어다 놓으면 펫/플레이어 인벤토리로 아이템이 옮겨집니다.  
인벤토리 위쪽을 좌클릭으로 끌어서 ui창을 이동할 수 있습니다.  


(디버깅용 LURD_TEST 비활성화 상태)  
zxcv: LURD 사이즈 조절  
IJKL: LURD 이동  

## 주요 코드
- ButtonUI
- Inventory
- ItemIcon
- ItemSlot

## 코드소개
<img width="801" height="616" alt="image" src="https://github.com/user-attachments/assets/df9e3722-6ff9-4b62-b46e-b601fe8d0378" />  
<img width="334" height="397" alt="image" src="https://github.com/user-attachments/assets/a81c23ed-1006-4c4c-a19e-867c2e870caa" />
<img width="142" height="142" alt="image" src="https://github.com/user-attachments/assets/56bc3306-024b-4c7f-bf08-19fdda45a1b9" />
<img width="131" height="141" alt="image" src="https://github.com/user-attachments/assets/e8706323-b953-4eec-b2df-515feabebedd" />

- 인벤토리, 아이템 슬롯, 아이템 아이콘으로 객체를 구성했습니다.  
- 인벤토리가 가진 데이터를 아이콘, 아이템슬롯과 동기화시키는 방식으로 객체를 관리했습니다.  
- 아이템 정렬, 정렬잠금, 펫 인벤토리, 아이템 이동/교체, 아이템 사용 기능을 구현했습니다.  
- 사운드는 FMOD를 사용했습니다.
- 펫과 플레이어의 인벤토리를 구분하기 위해 각각의 vector를 만들고, 활성화 상태에 따라 알맞는 vector를 참조하는 방식을 사용했습니다.  


