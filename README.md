# GRFramework (ImageLab at Unimore):

## Requisites:
- SDK 1.8 for the Kinect v1
- SDK 2.0 for the Kinect One (+ usb 3.0)
- OpenCV 2.4.11
- Qt 5.4.1 (+ Qt Creator)

## Features
### Devices:
  - Kinect v1 (RGB, depth, skeleton joints)
  - Kinect One v2 (RGB, depth, skeleton joints)
  
### AI techniques:
  - Dynamic Time Warping
  - Continuous Hidden Markov Model
  - Discrete Hidden Markov Model
  
### Contents from Kinect
- Kinect version
- RGB
- Depth
- Depth con skeleton
- Joints in Real World
- Joints RGB images
- Joints Depth images
- Head Pitch
- Head Roll
- Head Yaw

## Versions (italian)  
### 1.0:
- Starting version

#### 1.1: 
- [Ambiente di test skeleton] Angolo delle spalle calcolato in base al sistema Kinect, non più da angolo spalle primo frame
- [GUI Home] Pulsante per chiusura scheda corrente con ritorno all GUI Home

### 1.2: 
- [Frameset]: aggiunta depth senza skeleton
- [Ambiente di acquisizione]: frame salvati con numero di cifre fisso (xxxxxx.png)

### 2.0:
- [Device]: aggiunto supporto per Myo Armband
- [Engine]: aggiunto ambiente test Myo Armband
- [GUI]: aggiunto GUI Myo armband
- [Frameset]: aggiunto vettore dati EMG (8 muscoli) e dati orientazione (yaw, pitch, roll)

### 2.1
- [GUI Myo]: aggiunto grafico giroscopio (x, y, z)

### 2.1.1
- [Ambiente di acquisizione]: salvataggio dati Myo su file di testo
