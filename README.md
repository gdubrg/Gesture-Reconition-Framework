GRFramework (ImageLab at Unimore):

Requisiti:
- SDK 1.8 Kinect 1
- SDK 2.0 Kinect One (+ usb 3.0)
- OpenCV 2411
- Qt 5.4.1 (+ Qt Creator)
 
- Device:
  - Acquisizione da Kinect 1 (RGB, depth, joint)
  - Acquisizione da Kinect 2 (RGB, depth, joint)
  - Acquisizione da dataset (vedi codice)
- Engine:
  - Dynamic Time Warping
  - Hidden Markov Model
  - Ambiente di acquisizione (RGB, depth, joint)
  - Ambiente di test skeleton
- GUI:
  - GUI Home
  - GUI DTW
  - GUI HMM
  - GUI Test
  - GUI acquisizione
  - GUI Myo
- FrameSet (unità di acquisizione):
  - Kinect version
  - RGB
  - Depth
  - Depth con skeleton
  - Joint Real
  - Joint RGB
  - Joint Depth
  - Head Pitch
  - Head Roll
  - Head Yaw
  
1.0:
- Starting version

1.1: 
- [Ambiente di test skeleton] Angolo delle spalle calcolato in base al sistema Kinect, non più da angolo spalle primo frame
- [GUI Home] Pulsante per chiusura scheda corrente con ritorno all GUI Home

1.2: 
- [Frameset]: aggiunta depth senza skeleton
- [Ambiente di acquisizione]: frame salvati con numero di cifre fisso (xxxxxx.png)

2.0:
- [Device]: aggiunto supporto per Myo Armband
- [Engine]: aggiunto ambiente test Myo Armband
- [GUI]: aggiunto GUI Myo armband
- [Frameset]: aggiunto vettore dati EMG (8 muscoli) e dati orientazione (yaw, pitch, roll)

2.1
- [GUI Myo]: aggiunto grafico giroscopio (x, y, z)

2.1.1
- [Ambiente di acquisizione]: salvataggio dati Myo su file di testo
