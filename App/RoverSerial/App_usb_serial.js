import React, { Component } from "react";
import {
  StyleSheet,
  Text,
  View,
  TextInput,
  TouchableOpacity,
  ScrollView,
  Alert,
  DeviceEventEmitter
} from "react-native";
import Slider from '@react-native-community/slider';
import VerticalSlider from 'rn-vertical-slider';

import { RNSerialport, definitions, actions } from "react-native-serialport";
//type Props = {};
class App extends Component {
  constructor(props) {
    super(props);

    this.state = {
      servisStarted: false,
      connected: false,
      usbAttached: false,
      output: "",
      outputArray: [],
      baudRate: "9600",
      interface: "-1",
      sendText: " ",
      speed: 0,
      speedR: 0,
      speedL: 0,
      returnedDataType: definitions.RETURNED_DATA_TYPES.HEXSTRING
    };

    this.startUsbListener = this.startUsbListener.bind(this);
    this.stopUsbListener = this.stopUsbListener.bind(this);
  }

  componentDidMount() {
    this.startUsbListener();
  }

  componentWillUnmount() {
    this.stopUsbListener();
  }

  startUsbListener() {
    DeviceEventEmitter.addListener(
      actions.ON_SERVICE_STARTED,
      this.onServiceStarted,
      this
    );
    DeviceEventEmitter.addListener(
      actions.ON_SERVICE_STOPPED,
      this.onServiceStopped,
      this
    );
    DeviceEventEmitter.addListener(
      actions.ON_DEVICE_ATTACHED,
      this.onDeviceAttached,
      this
    );
    DeviceEventEmitter.addListener(
      actions.ON_DEVICE_DETACHED,
      this.onDeviceDetached,
      this
    );
    DeviceEventEmitter.addListener(actions.ON_ERROR, this.onError, this);
    DeviceEventEmitter.addListener(
      actions.ON_CONNECTED,
      this.onConnected,
      this
    );
    DeviceEventEmitter.addListener(
      actions.ON_DISCONNECTED,
      this.onDisconnected,
      this
    );
    DeviceEventEmitter.addListener(actions.ON_READ_DATA, this.onReadData, this);
    RNSerialport.setReturnedDataType(this.state.returnedDataType);
    RNSerialport.setAutoConnectBaudRate(parseInt(this.state.baudRate, 10));
    RNSerialport.setInterface(parseInt(this.state.interface, 10));
    RNSerialport.setAutoConnect(true);
    RNSerialport.startUsbService();
  };

  stopUsbListener = async () => {
    DeviceEventEmitter.removeAllListeners();
    const isOpen = await RNSerialport.isOpen();
    if (isOpen) {
      Alert.alert("isOpen", isOpen);
      RNSerialport.disconnect();
    }
    RNSerialport.stopUsbService();
  };

  onServiceStarted(response) {
    this.setState({ servisStarted: true });
    if (response.deviceAttached) {
      this.onDeviceAttached();
    }
  }
  onServiceStopped() {
    this.setState({ servisStarted: false });
  }
  onDeviceAttached() {
    this.setState({ usbAttached: true });
  }
  onDeviceDetached() {
    this.setState({ usbAttached: false });
  }
  onConnected() {
    this.setState({ connected: true });
  }
  onDisconnected() {
    this.setState({ connected: false });
  }
  onReadData(data) {
    if (
      this.state.returnedDataType === definitions.RETURNED_DATA_TYPES.INTARRAY
    ) {
      const payload = RNSerialport.intArrayToUtf16(data.payload);
      this.setState({ output: this.state.output + payload });
    } else if (
      this.state.returnedDataType === definitions.RETURNED_DATA_TYPES.HEXSTRING
    ) {
      const payload = RNSerialport.hexToUtf16(data.payload);
      this.setState({ output: this.state.output + payload });
    }
  }

  onError(error) {
    console.error(error);
  }

  handleConvertButton() {
    let data = "";
    if (
      this.state.returnedDataType === definitions.RETURNED_DATA_TYPES.HEXSTRING
    ) {
      data = RNSerialport.hexToUtf16(this.state.output);
    } else if (
      this.state.returnedDataType === definitions.RETURNED_DATA_TYPES.INTARRAY
    ) {
      data = RNSerialport.intArrayToUtf16(this.state.outputArray);
    } else {
      return;
    }
    this.setState({ output: data });
  }

  handleClearButton() {
    this.setState({ output: "" });
    this.setState({ outputArray: [] });
  }

  handleSendButton(){
    RNSerialport.writeString(this.state.sendText);
  }
  
  setSpeed(speed){
    this.setState({speed:speed/255*100});
    RNSerialport.writeString('SPEED'+speed+'\n');
  }

  setSpeedR(speed){
    this.setState({speedR:(speed/255*100).toFixed(0)});
    RNSerialport.writeString('R'+speed+'\n');
  }

  setSpeedL(speed){
    this.setState({speedL:(speed/255*100).toFixed(0)});
    RNSerialport.writeString('L'+speed+'\n');
    
  }

  setInitSpeedL(){
    speed = 0;
    this.setState({speedL:0});
    RNSerialport.writeString('L'+speed+'\n');

  }

  setInitSpeedR(){
    speed = 0;
    this.setState({speedR:0});
    RNSerialport.writeString('R'+speed+'\n');
  }


  forward(){
    RNSerialport.writeString('W'+'\n');
  }
  back(){ 
    RNSerialport.writeString('S'+'\n');
  }
  left(){
    RNSerialport.writeString('A'+'\n');
  }
  right(){
    RNSerialport.writeString('D'+'\n');
  }
  stop(){
    RNSerialport.writeString('X'+'\n');
  }

  buttonStyle = status => {
    return status
      ? styles.button
      : Object.assign({}, styles.button, { backgroundColor: "#C0C0C0" });
  };

  render() {
    return (
      <ScrollView style={styles.body}>
        <View style={styles.container}>
          <View style={styles.header}>
            <View style={styles.line}>
              <Text style={styles.title}>Service:</Text>
              <Text style={styles.value}>
                {this.state.servisStarted ? "Started" : "Not Started"}
              </Text>
            </View>
            <View style={styles.line}>
              <Text style={styles.title}>Usb:</Text>
              <Text style={styles.value}>
                {this.state.usbAttached ? "Attached" : "Not Attached"}
              </Text>
            </View>
            <View style={styles.line}>
              <Text style={styles.title}>Connection:</Text>
              <Text style={styles.value}>
                {this.state.connected ? "Connected" : "Not Connected"}
              </Text>
            </View>
          </View>
          <ScrollView style={styles.output} nestedScrollEnabled={true}>
            <Text style={styles.full}>
              {this.state.output === "" ? "No Content" : this.state.output}
            </Text>
          </ScrollView>
{/* 
          <View style={styles.inputContainer}>
            <Text>Send</Text>
            <TextInput
              style={styles.textInput}
              onChangeText={text => this.setState({ sendText: text })}
              value={this.state.sendText}
              placeholder={"Send Text"}
            />
          </View> */}
          <View style={styles.line2}>
            {/* <TouchableOpacity
              style={this.buttonStyle(this.state.connected)}
              onPress={() => this.handleSendButton()}
              disabled={!this.state.connected}
            >
              <Text style={styles.buttonText}>Send</Text>
            </TouchableOpacity> */}
            <TouchableOpacity
              style={styles.button}
              onPress={() => this.handleClearButton()}
            >
              <Text style={styles.buttonText}>Clear</Text>
            </TouchableOpacity>
            {/* <TouchableOpacity
              style={styles.button}
              onPress={() => this.handleConvertButton()}
            >
              <Text style={styles.buttonText}>Convert</Text>
            </TouchableOpacity> */}
          </View>

          <View style={styles.line2}>
             <TouchableOpacity
              style={this.buttonStyle(this.state.connected)}
              onPressIn={() => this.forward()}
              onPressOut={() => this.stop()}
              disabled={!this.state.connected}
            >
              <Text style={styles.buttonText}>Avanti</Text>
            </TouchableOpacity> 

            <TouchableOpacity
              style={this.buttonStyle(this.state.connected)}
              onPressIn={() => this.back()}
              onPressOut={() => this.stop()}
              disabled={!this.state.connected}
            >
              <Text style={styles.buttonText}>Indietro</Text>
            </TouchableOpacity> 

            <TouchableOpacity
              style={this.buttonStyle(this.state.connected)}
              onPressIn={() => this.left()}
              onPressOut={() => this.stop()}
              disabled={!this.state.connected}
            >
              <Text style={styles.buttonText}>Sinistra</Text>
            </TouchableOpacity> 

            <TouchableOpacity
              style={this.buttonStyle(this.state.connected)}
              onPressIn={() => this.right()}
              onPressOut={() => this.stop()}
              disabled={!this.state.connected}
            >
              <Text style={styles.buttonText}>Destra</Text>
            </TouchableOpacity> 
 

          </View>
          <View style={styles.line2}>

           <Slider
    style={{width: 500, height: 40}}
    minimumValue={0}
    maximumValue={255}
    minimumTrackTintColor="#00000"
    maximumTrackTintColor="#00000"
    onSlidingComplete={ (value) => this.setSpeed(value.toFixed(0))}
    onValueChange={(value) => this.setState({speed:value.toFixed(0)})}
    step={2}
    disabled={!this.state.connected}

  /> 
  {/* <Text>Velocità: {this.state.speed}</Text> */}

              </View>
              <View style={styles.verticalSlidersTitle}>

              <Text>Velocità Sinistra: {this.state.speedL}</Text>
              <Text>Velocità Destra: {this.state.speedR}</Text>
              </View>
              <View style={styles.verticalSliders}>



  {/* <VerticalSlider
          value={0}
          disabled={false}
          min={0}
          max={255}
          onChange={(value) => {this.setSpeedL(value.toFixed(0))}}
          onComplete={(value) => {this.setSpeedL(value.toFixed(0))}}
          width={50}
          height={300}
          step={2}
          borderRadius={5}
          minimumTrackTintColor={"red"}
          maximumTrackTintColor={"black"}
          //showBallIndicator
          ballIndicatorColor={"gray"}
          ballIndicatorTextColor={"white"}
*/}
        <Slider
    style={{width: 250, height: 50, transform: [{ rotate: '-90deg' }]}}
    minimumValue={0}
    maximumValue={255}
    minimumTrackTintColor="#00000"
    maximumTrackTintColor="#00000"
    // onSlidingComplete={ (value) => this.setSpeedL(value.toFixed(0))}
     onSlidingComplete={ (value) => this.setInitSpeedL()}
    onValueChange={(value) => this.setSpeedL(value.toFixed(0))}
    step={2}
    disabled={!this.state.connected}
    value={parseInt(this.state.speedL)}

    /> 

{/* <VerticalSlider
          value={0}
          disabled={false}
          min={0}
          max={255}
          onChange={(value) => {this.setSpeedR(value.toFixed(0))}}
          onComplete={(value) => {this.setSpeedR(value.toFixed(0))}}
          width={50}
          height={300}
          step={2}
          borderRadius={5}
          minimumTrackTintColor={"red"}
          maximumTrackTintColor={"black"}
          //showBallIndicator
          ballIndicatorColor={"gray"}
          ballIndicatorTextColor={"white"}
        /> */}

<Slider
    style={{width: 250, height: 50, transform: [{ rotate: '-90deg' }]}}
    minimumValue={0}
    maximumValue={255}
    minimumTrackTintColor="#00000"
    maximumTrackTintColor="#00000"
    // onSlidingComplete={ (value) => this.setSpeedL(value.toFixed(0))}
     onSlidingComplete={ () => this.setInitSpeedR()}
    onValueChange={(value) => this.setSpeedR(value.toFixed(0))}
    step={2}
    disabled={!this.state.connected}
    value={parseInt(this.state.speedR)}

    /> 
   
  </View>
  </View>
      </ScrollView>
    );
  }
}

const styles = StyleSheet.create({
  full: {
    flex: 1
  },
  body: {
    flex: 1
  },
  container: {
    flex: 1,
    marginTop: 20,
    marginLeft: 16,
    marginRight: 16
  },
  header: {
    display: "flex",
    justifyContent: "center"
    //alignItems: "center"
  },
  line: {
    display: "flex",
    flexDirection: "row"
  },
  line2: {
    display: "flex",
    flexDirection: "row",
    justifyContent: "space-between"
  },
  verticalSliders: {
    display: "flex",
    flexDirection: "row",
    justifyContent: "space-between",
    height:150,
    marginTop:100
    },  
    
    verticalSlidersTitle: {
      display: "flex",
      flexDirection: "row",
      justifyContent: "space-between",

      },
  title: {
    width: 100
  },
  value: {
    marginLeft: 20
  },
  output: {
    marginTop: 10,
    height: 300,
    padding: 10,
    backgroundColor: "#FFFFFF",
    borderWidth: 1
  },
  inputContainer: {
    marginTop: 10,
    borderBottomWidth: 2
  },
  textInput: {
    paddingLeft: 10,
    paddingRight: 10,
    height: 40
  },
  button: {
    marginTop: 16,
    marginBottom: 16,
    paddingLeft: 15,
    paddingRight: 15,
    height: 40,
    justifyContent: "center",
    alignItems: "center",
    backgroundColor: "#147efb",
    borderRadius: 3
  },
  buttonText: {
    color: "#FFFFFF"
  }
});

export default App;