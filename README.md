Versioni:

1.0: 
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
- FrameSet (unità di acquisizione):
  - Kinect version
  - RGB
  - Depth con skeleton
  - Joint Real
  - Joint RGB
  - Joint Depth
  - Head Pitch
  - Head Roll
  - Head Yaw

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
- [Frameset]: aggiunto vettore dati EMG (8 muscoli) e dati giroscopio (yaw, pitch, roll)
