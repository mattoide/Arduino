import React, { Component } from "react";
import {
  StyleSheet,
  Text,
  View,
  TouchableOpacity,
  ScrollView,
  PermissionsAndroid,
  ActivityIndicator,
  ToastAndroid
} from "react-native";
import Slider from '@react-native-community/slider';
import BluetoothSerial from 'react-native-bluetooth-serial-next';
import BackgroundTimer from 'react-native-background-timer';
import Modal from './src/modal';


import MaterialIcons from 'react-native-vector-icons/MaterialIcons';
import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';
import FeatherIcons from 'react-native-vector-icons/Feather';
import AntDesign from 'react-native-vector-icons/AntDesign';

const initialState = {
  output: "",
  outputArray: [],
  drivingMode: "pad",
  speed: 0,
  speedR: 0,
  speedL: 0,
  isBtEnabled: false,
  isBtConnected: false,
  devices: [],
  connectedDevice: {},
  pairedDevices: [],
  unPairedDevices: [],
  discoveredDevices: [],
  devicesFormatted: '',
  modalVisible: false,
  loading: false,
  timerInterval: 3
};


class App extends Component {
  constructor(props) {
    super(props);
    this.state = initialState;
  }



  resetState() {
    this.setState(initialState);
    this.checkBtEnabledAndConnected();

  }

  componentDidMount() {
    this.checkBtEnabledAndConnected();

    BluetoothSerial.on('bluetoothEnabled', () => {
      this.setState({ isBtEnabled: true })
    });

    BluetoothSerial.on('bluetoothDisabled', () => {
      this.setState({ isBtEnabled: false })
      this.resetState();
    });

    BluetoothSerial.on('connectionLost', (connectionLost) => {
      this.setState({ isBtConnected: false })
      this.resetState();
    });

    BluetoothSerial.on('connectionSuccess', () => {
      this.setState({ isBtConnected: true })
      this.setSpeed(0);
      BluetoothSerial.read((data, subscription) => {

        if (this.imBoredNow && subscription) {
          BluetoothSerial.removeSubscription(subscription);
        }
      }, "\r\n");
    });

    BluetoothSerial.on('data', (data) => {
      //console.log(data);
      this.setState({ output: data.data })
    });

    BluetoothSerial.on('error', (error) => {
      console.log(error);
    });

  }

  switchDrivingMode(drivingMode) {
    this.setState({ drivingMode: drivingMode });
  }



  async checkBtEnabledAndConnected() {
    let isBtEnabled = await BluetoothSerial.isEnabled();
    let isBtConnected = await BluetoothSerial.isConnected();

    this.setState({ isBtEnabled: isBtEnabled, isBtConnected: isBtConnected })
  }

  async listPairedDevices() {
    let pairedDevices = await BluetoothSerial.list();
    // this.setState({pairedDevices: pairedDevices})
    this.setState({ devices: pairedDevices })
    this.setState({ modalVisible: true });
  }

  async listUnPairedDevices() {
    PermissionsAndroid.check(PermissionsAndroid.PERMISSIONS.ACCESS_COARSE_LOCATION)
      .then(result => {
        if (!result) {
          PermissionsAndroid.request(PermissionsAndroid.PERMISSIONS.ACCESS_COARSE_LOCATION);
        }
      });
    let unPairedDevices = await BluetoothSerial.listUnpaired();
    // this.setState({unPairedDevices: unPairedDevices})
    this.setState({ devices: unPairedDevices })
    this.setState({ modalVisible: true });

  }

  async discoverDevices() {
    let pairedDevices = await BluetoothSerial.list();

    PermissionsAndroid.check(PermissionsAndroid.PERMISSIONS.ACCESS_COARSE_LOCATION)
      .then(result => {
        if (!result) {
          PermissionsAndroid.request(PermissionsAndroid.PERMISSIONS.ACCESS_COARSE_LOCATION);
        } else {
          if (this.state.isBtEnabled) {

            //this.setState({devices:[]});
            this.setState({ loading: true })
            // const devices = await BluetoothSerial.discoverUnpairedDevices();
            BluetoothSerial.discoverUnpairedDevices().then(

              devices => {

                this.setState({ devices: devices })
                this.setState({ loading: false })
                  // this.setState({modalVisible:true})

                  , error => {
                    this.setState({ loading: false })
                    console.log("error")
                    console.log(error)
                  }
              });

          } else {
            this.toast("Accendi il bluetooth!");
          }
        }
      });

  }

  toast(message) {
    ToastAndroid.showWithGravity(
      message,
      ToastAndroid.SHORT,
      ToastAndroid.CENTER,
    );
  }
  getDeviceToConnect = (deviceToConnect) => {
    //this.setState({connectedDevice: deviceToConnect})
    if (!deviceToConnect) {
      //this.setState({modalVisible:false, devices:[]})
      // this.state.devices = [];
      this.setState({ devices: [] })

    } else {
      this.setState({ loading: true })
      this.connectToDevice(deviceToConnect.id);
    }

  }


  async connectToDevice(id) {

    // BluetoothSerial.connect('98:D3:31:F5:3F:40')
    BluetoothSerial.connect(id)
      .then(connectedDevice => {
        this.setState({ loading: false })

        this.setState({ connectedDevice: connectedDevice, devices: [], modalVisible: false })

      }, error => {

        this.toast("Impossibile connettersi.");
      });

  }

  async disconncectFromDevice(id) {
    let disconnectFrom = await BluetoothSerial.disconnect();
  }

  async disconncectFromAllDevices() {
    let disconnectFromAll = await BluetoothSerial.disconnectAll();
    if (disconnectFromAll)
      this.setState({ connectedDevice: {} })
  }

  async isConnected() {
    const connectedDevice = await BluetoothSerial.isConnected();
    this.setState({ connectedDevice: connectedDevice })
  }

  onError(error) {
    console.error(error);
  }

  handleClearButton() {
    this.setState({ output: "" });
    this.setState({ outputArray: [] });
  }

  handleSendButton() {
    BluetoothSerial.write(this.state.sendText);
  }

  setSpeed(speed) {
    this.setState({ speed: speed });
    BluetoothSerial.write("F" + speed + '\n');
  }



  forwardLeft() {

    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("Q" + '\n');
    },
      this.state.timerInterval);
  }


  forwardRight() {
    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("E" + '\n');
    },
      this.state.timerInterval);
  }


  backwardLeft() {
    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("Z" + '\n');
    },
      this.state.timerInterval);  }


  backwardRight() {
    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("C" + '\n');
    },
      this.state.timerInterval);  }


  forward() {

    //BluetoothSerial.write("W"+'\n');

    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("W" + '\n');
    },
      this.state.timerInterval);
  }
  backward() {
    //BluetoothSerial.write("S"+'\n');
    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("S" + '\n');
    },
      this.state.timerInterval);
  }
  right() {
    //BluetoothSerial.write("D"+'\n');
    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("D" + '\n');
    },
      this.state.timerInterval);
  }
  left() {
    //BluetoothSerial.write("A"+'\n');
    BackgroundTimer.runBackgroundTimer(() => {
      BluetoothSerial.write("A" + '\n');
    },
      this.state.timerInterval);
  }
  stop() {
    BackgroundTimer.stopBackgroundTimer();
    BluetoothSerial.write("X" + '\n');
  }

  buttonStyle = status => {
    return status
      ? styles.button
      : Object.assign({}, styles.button, { backgroundColor: "#C0C0C0" });
  };

  render() {
    return (


      <ScrollView style={styles.body}>
        <Modal
          //modalVisible={this.state.modalVisible}
          devices={this.state.devices}
          connectedDevice={this.state.connectedDevice}
          connectTo={this.getDeviceToConnect}
        ></Modal>
        <View style={this.state.loading ? styles.container : styles.containerLoading} pointerEvents={this.state.loading ? 'none' : 'auto'} >

          {this.state.loading ?
            <View style={styles.loading} >
              <ActivityIndicator size={100} color="black" />
            </View> : undefined}

          {/* Info */}
          <View style={styles.infoContainer}>
            <View style={styles.header}>
              <View style={styles.line}>
                <Text style={styles.title}>Bluetooth:</Text>
                <Text style={styles.value}>
                  {this.state.isBtEnabled ? "Attivo" : "Non attivo"}
                </Text>
              </View>
              <View style={styles.line}>
                <Text style={styles.title}>Stato:</Text>
                <Text style={styles.value}>
                  {this.state.isBtConnected ? "Connesso a " + this.state.connectedDevice.name : "Non connesso"}
                </Text>
              </View>
            </View>
            <ScrollView style={styles.output} nestedScrollEnabled={true}>
              <Text style={styles.full}>
                {this.state.output === "" ? "No Content" : this.state.output}
              </Text>
            </ScrollView>
          </View>

          {/* Bluetooth commmands */}

          <View style={styles.btCommandContainer}>
            {/* <TouchableOpacity
              style={styles.button}
              onPress={() => this.connectToDevice('98:D3:31:F5:3F:40')}
            >
              <Text style={styles.buttonText}>Connect to</Text>
            </TouchableOpacity> */}
            {/* <TouchableOpacity
              style={styles.button}
              onPress={() => this.handleClearButton()}
            >
              <Text style={styles.buttonText}>Clear</Text>
            </TouchableOpacity>

 


            <TouchableOpacity
              style={styles.button}
              onPress={() => this.listPairedDevices()}
            >
              <Text style={styles.buttonText}>List paired Devices</Text>
            </TouchableOpacity>
           
          <TouchableOpacity
              style={styles.button}
              onPress={() => this.listUnPairedDevices()}
            >
              <Text style={styles.buttonText}>List UNPAIRED Devices</Text>
            </TouchableOpacity> */}

            <TouchableOpacity
              // onPress={() => this.discoverDevices()}
              onPress={() => this.listPairedDevices()}

            >
              {/* <Text style={styles.buttonText}>Cerca dispositivi</Text> */}
              <MaterialIcons name="bluetooth-searching" size={50} />

            </TouchableOpacity>

            {/* <TouchableOpacity
              style={styles.button}
              onPress={() => this.connectToDevice()}
            >
              <Text style={styles.buttonText}>Connect to</Text>
            </TouchableOpacity> */}

            {/* <TouchableOpacity
              style={styles.button}
              onPress={() => this.disconncectFromDevice("")}
            >
              <Text style={styles.buttonText}>Disconnect from</Text>
            </TouchableOpacity>
        </View> */}


            <TouchableOpacity
              onPress={() => this.disconncectFromAllDevices()}
            >
              <MaterialCommunityIcons name="lan-disconnect" size={50} />
            </TouchableOpacity>


            {/* <TouchableOpacity
              style={styles.button}
              onPress={() => this.send()}
            >
              <Text style={styles.buttonText}>INVIA</Text>
            </TouchableOpacity> */}
          </View>


          <View style={styles.padStyleS}>

            <Text>Velocità: {(this.state.speed / 255 * 100).toFixed(0)} %</Text>

            <Slider
              minimumValue={0}
              maximumValue={255}
              minimumTrackTintColor="#00000"
              maximumTrackTintColor="#00000"
              onValueChange={(value) => this.setState({ speed: value })}
              onSlidingComplete={(value) => this.setSpeed(value.toFixed(0))}
              step={2}
              disabled={!this.state.isBtConnected}
              value={parseFloat(this.state.speed)}

            />

          </View>
          <View style={styles.padStyle}>
            <View style={styles.padStyleL}>
              <TouchableOpacity
                style={styles.upLeft}

                //style={this.buttonStyle(this.state.connected)}
                onPressIn={() => this.forwardLeft()}
                onPressOut={() => this.stop()}
                disabled={!this.state.isBtConnected}
              >
                {/* <Text style={styles.buttonText}>Avanti</Text> */}
                <AntDesign name="caretleft" size={100} />


              </TouchableOpacity>

              <TouchableOpacity
                //style={this.buttonStyle(this.state.connected)}
                onPressIn={() => this.forward()}
                onPressOut={() => this.stop()}
                disabled={!this.state.isBtConnected}
              >
                {/* <Text style={styles.buttonText}>Indietro</Text> */}
                <AntDesign name="caretup" size={100} />

              </TouchableOpacity>

              <TouchableOpacity
                style={styles.upRight}
                //style={this.buttonStyle(this.state.connected)}
                onPressIn={() => this.forwardRight()}
                onPressOut={() => this.stop()}
                disabled={!this.state.isBtConnected}
              >
                {/* <Text style={styles.buttonText}>Indietro</Text> */}
                <AntDesign name="caretright" size={100} />

              </TouchableOpacity>
            </View>
            <View style={styles.padStyleR}>

              <TouchableOpacity

                style={styles.downLeft}

                onPressIn={() => this.backwardLeft()}
                onPressOut={() => this.stop()}
                disabled={!this.state.isBtConnected}
              >
                <AntDesign name="caretleft" size={100} />

              </TouchableOpacity>

              <TouchableOpacity
                onPressIn={() => this.backward()}
                onPressOut={() => this.stop()}
                disabled={!this.state.isBtConnected}
              >
                <AntDesign name="caretdown" size={100} />

              </TouchableOpacity>

              <TouchableOpacity
                style={styles.downRight}

                onPressIn={() => this.backwardRight()}
                onPressOut={() => this.stop()}
                disabled={!this.state.isBtConnected}
              >
                <AntDesign name="caretright" size={100} />

              </TouchableOpacity>
            </View>

          </View>


        </View>


      </ScrollView>
    );
  }
}

const styles = StyleSheet.create({
  full: {
    flex: 1,
  },
  body: {
    flex: 1,
  },
  container: {
    flex: 1,
    marginTop: 15,
    marginLeft: 10,
    marginRight: 10,
  },
  containerLoading: {
    flex: 1,
    marginTop: 15,
    marginLeft: 10,
    marginRight: 10,
    opacity: 0.8
  },
  infoContainer: {
    flex: 1,
  },
  btCommandContainer: {
    display: "flex",
    flexDirection: "row",
    justifyContent: "space-between"
  },
  drivingModeContainer: {
    display: "flex",
    flexDirection: "row",
    justifyContent: "center",
    marginVertical: 50
  },
  loading: {
    position: 'absolute',
    top: 0, left: 0,
    right: 0, bottom: 0,
    justifyContent: 'center',
    alignItems: 'center'
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
  padStyle: {
    display: "flex",
    flexDirection: "column",
    justifyContent: "space-between",
    marginTop: 50

  },
  padStyleL: {
    display: "flex",
    flex: 0.3,
    flexDirection: "row",
    justifyContent: "space-between",
  },
  padStyleR: {
    display: "flex",
    flex: 0.5,
    flexDirection: "row",
    justifyContent: "space-around",
    alignItems: "center",
  },
  padStyleS: {
    display: "flex",
    justifyContent: "center",
    marginTop: 50
  },


  slidersStyle: {
    flex: 1,
    display: "flex",
    flexDirection: "row",

  },
  slidersStyleL: {
    display: "flex",
    flex: 0.5,
    flexDirection: "column",
    alignItems: 'center'

  },
  slidersStyleR: {
    display: "flex",
    flex: 0.5,
    flexDirection: "column",
    alignItems: 'center'

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
    height: 150,
    marginTop: 100
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
    flex: 0.2,
    marginTop: 10,
    height: 100,
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
  },

  upLeft: {
    transform: [{ rotate: '45deg' }]
  },
  upRight: {
    transform: [{ rotate: '-45deg' }],
  },
  downLeft: {
    transform: [{ rotate: '-45deg' }],
  },
  downRight: {
    transform: [{ rotate: '45deg' }],
  }
});

export default App;

// export default withSubscription({
//   subscriptionName: 'events',
//   destroyOnWillUnmount: true,
// })(App);