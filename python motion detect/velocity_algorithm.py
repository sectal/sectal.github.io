import Tkinter as tk
from Tkinter import *
import cv2
import time
import numpy as np
import datetime
from time import gmtime, strftime
import sys
import os
import csv

def changing_file():#updates the video
    cap = cv2.VideoCapture('background.avi')
    window_name = "Window Test"
    cv2.namedWindow(window_name,cv2.WINDOW_NORMAL)
    cv2.resizeWindow(window_name,1280,720)
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    out = cv2.VideoWriter('recording_resized.avi',fourcc, 20.0, (320,240))
    frame = 0
    while(cap.isOpened()):
        ret, frame = cap.read()
        if frame is None:
            break
        out.write(frame)
        cv2.imshow(window_name,frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    os.remove("background.avi")

def miles_per_hour(diff, start,end,add,video):#speed of the car
    miles = float(entry.get())/5280#conversion from feet to miles
    mph = miles/diff
    label2 = tk.Label(root, text=str(mph),font=("Helvetica", 12))
    label2.grid(row=5+add, column = 0)
    label3 = tk.Label(root, text=str(start)+"   ",font=("Helvetica", 12))
    label3.grid(row=5+add, column = 1)
    label4 = tk.Label(root, text=str(end),font=("Helvetica", 12))
    label4.grid(row=5+add, column = 2)
    label7 = tk.Label(root, text= "Click the video window and press escape to end the program",font=("Helvetica", 10))
    label7.grid(row=3, column = 1)
    #not working properly, callbacks don't work like s= s+1
    with open("velocity.txt","w") as scoreFile:#writing to that txt file
        scoreFileWriter = csv.writer(scoreFile)
        scoreFileWriter.writerow(["Entry: " + entry.get()])
        scoreFileWriter.writerow(["Velocity         " "Start Time     " "End Time"])
        scoreFileWriter.writerow([mph, start, end])
    
    #label2.config(text ='velocity: '+ str(mph),height = 1)
    #label3.config(text = "start time: "+str(start))
    #label4.config(text = "end time: "+str(end))
    root.update()


def diffImg(t0, t1, t2):#subtracting the images
  d1 = cv2.absdiff(t2, t1)#absolute difference between two frames
  d2 = cv2.absdiff(t1, t0)#absolute difference between the other two frames
  return cv2.bitwise_and(d1, d2)#AND

def test():#program start
  cam = cv2.VideoCapture(0)#camera1 starts working at location 0
  cam.set(3,160)#setting the width and height
  cam.set(4,120)


  cam1 = cv2.VideoCapture(1)#camera2 starts
  cam1.set(3,160)#same dimensions
  cam1.set(4,120)
  winName = "Movement Indicator"
  winName2 = "Movement Indicator2"


  # Read three images first:
  t_minus = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)#get three images from each camera and save them
  t = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)
  t_plus = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)

  t_minus1 = cv2.cvtColor(cam1.read()[1], cv2.COLOR_RGB2GRAY)
  t1 = cv2.cvtColor(cam1.read()[1], cv2.COLOR_RGB2GRAY)
  t_plus1 = cv2.cvtColor(cam1.read()[1], cv2.COLOR_RGB2GRAY)

  start_timer = 0
  end_timer = 0
  label_maker = 0
  fourcc = cv2.VideoWriter_fourcc(*'XVID')#codec for .avi video
  out = cv2.VideoWriter('background.avi',fourcc, 20.0, (320,240))#download video

  while True:

    #start = time.time()
    ret,frame = cam.read()#camera1 is read 
    ret2,frame2 = cam1.read()#camera 2 is read
    both_normal = np.hstack((frame,frame2))#putting the frames side by side
    both_normal = cv2.cvtColor(both_normal, cv2.COLOR_BGR2GRAY)#making it go from color to gray
    
    num = 0#initializing variables
    num1 = 0
    timer = 0
    
    both= np.hstack((diffImg(t_minus, t, t_plus),diffImg(t_minus1, t1, t_plus1)))#the difference in images side by side
    time_now = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3] #get the time
    
    #timer system starts
    for y in range (1,120):#number of rows so |
      for x in range(54,106):# thickness of the line -(380 is the greatest number)
        if both[y][x]>0:#someone is passed that point in the camera
          num =num +1
          if num==2600 and start_timer==0:#num is the sensitivity of the capture
            start_timer = time.time()#get starting time
            start_time_now = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3] #tje format
            

    for y in range (1,120):#someone stepped out of the camera
      for x in range(216,268):
        if both[y][x] >0:
          num1 = num1+1
          if num1==2600 and end_timer==0:
            end_timer =time.time()
            end_time_now = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3] 
          
    if end_timer>0:#get the time difference to calculate the speed
      if start_timer>0:
        if end_timer>start_timer:
          timer = end_timer-start_timer
          #cv2.destroyWindow(winName)
          miles_per_hour(timer, start_time_now, end_time_now,label_maker,both)
          label_maker = 1+label_maker
          #cv2.destroyWindow(winName)
          #cam.release()
          #cam1.release()
          #break
          start_timer =0
          end_timer == 0
        if start_timer>=end_timer:
          timer = 0
          start_timer = 0
          end_timer = 0
    #timer system ends

    both= np.vstack((both_normal,both))#put images together top and bottom
    cv2.putText(both, "Time: "+(time_now), (5, 120), cv2.FONT_HERSHEY_PLAIN, 0.8, (255,255,255))#keeps updating
    cv2.putText(both, "Time: "+(time_now), (155, 120), cv2.FONT_HERSHEY_PLAIN, 0.8, (255,255,255))
    cv2.line(both,(54,1),(54,240),(255,0,0),2)
    cv2.line(both,(106,1),(106,240),(255,0,0),2)
    cv2.line(both,(216,1),(216,240),(255,0,0),2)
    cv2.line(both,(268,1),(268,240),(255,0,0),2)
    out.write(both)
    cv2.imshow( winName, both )
    

    # Read next image
    t_minus = t
    t = t_plus
    t_plus = cv2.cvtColor(cam.read()[1], cv2.COLOR_RGB2GRAY)#camera 1 updates

    t_minus1 = t1
    t1 = t_plus1
    t_plus1 = cv2.cvtColor(cam1.read()[1], cv2.COLOR_RGB2GRAY)#camera 2 updates
    #end = time.time()
    #seconds = end - start
    #fps = 1/seconds
    #print fps

    key = cv2.waitKey(10)#wait until key is pressed to stop program
    if key == 27:
      root.destroy()
      cam.release()
      cam1.release()
      cv2.destroyWindow(winName)
      break

    

root = tk.Tk()#tells you the end statistics
root.title("Interface")
label = tk.Label(root, text='Input track length:',font=("Comic Sans MS", 16,"bold"))
label.grid(row = 0,column=1, sticky=N)
entry = tk.Entry(root)
entry.grid(row = 1, column=1)
label5 = tk.Label(root, text='Velocity',font=("Comic Sans MS", 14,"bold"))
label5.grid(row=4, column = 0)
label6 = tk.Label(root, text='Start Time     ',font=("Comic Sans MS", 14,"bold"))
label6.grid(row=4, column = 1)
label6 = tk.Label(root, text='End Time',font=("Comic Sans MS", 14,"bold"))
label6.grid(row=4, column = 2)
b = Button(root, text="miles per hour", width=10, command=lambda: test())
b.grid(row = 2,column =1)
#root.geometry("450x450 +500+300")

root.mainloop()#ends root
changing_file()

