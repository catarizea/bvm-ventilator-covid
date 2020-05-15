import React, { PureComponent } from 'react';
import { SafeAreaView, StyleSheet, View } from 'react-native';
import { Button } from 'react-native-paper';
import PropTypes from 'prop-types';

import ScreenContainer from '../components/ScreenContainer';
import RangeSlider from '../components/RangeSlider';

class SettingsScreen extends PureComponent {
  constructor(props) {
    super(props);

    this.state = {
      settings: [600, 15, 1, 2],
    };

    this.handlerRange = this.handlerRange.bind(this);
    this.handlerButton = this.handlerButton.bind(this);
  }

  handlerRange(value, fromUser, type) {
    const newSettings = [...this.state.settings];
    const newValue = parseInt(value, 10);

    switch (type) {
      case 'volume':
        newSettings[0] = newValue;
        break;
      case 'rate':
        newSettings[1] = newValue;
        break;
      case 'inspiration':
        newSettings[2] = newValue;
        break;
      case 'expiration':
        newSettings[3] = newValue;
        break;
    }

    this.setState({ settings: newSettings });
  }

  handlerButton(type) {
    if (type === 'stop') {
      this.props.writeNewSettings([0, 0, 0, 0]);
    }

    if (type === 'start') {
      this.props.writeNewSettings(this.state.settings);
    }
  }

  render() {
    const { settings } = this.state;

    return (
      <ScreenContainer>
        <>
          <View style={styles.spaceUp} />
          <SafeAreaView style={styles.main}>
            <RangeSlider
              key="volume"
              min={100}
              max={1600}
              step={50}
              initialLowValue={settings[0]}
              handler={(low, fromUser) =>
                this.handlerRange(low, fromUser, 'volume')
              }
              label="Tidal Volume"
              unit="ml"
            />
            <RangeSlider
              key="rate"
              min={10}
              max={30}
              step={1}
              initialLowValue={settings[1]}
              handler={(low, fromUser) =>
                this.handlerRange(low, fromUser, 'rate')
              }
              label="Respiratory Rate"
              unit="per minute"
            />
            <RangeSlider
              key="inspiration"
              min={1}
              max={5}
              step={1}
              initialLowValue={settings[2]}
              handler={(low, fromUser) => {
                this.handlerRange(low, fromUser, 'inspiration');
              }}
              label="Inspiration Ratio"
            />
            <RangeSlider
              key="expiration"
              min={1}
              max={5}
              step={1}
              initialLowValue={settings[3]}
              handler={(low, fromUser) => {
                this.handlerRange(low, fromUser, 'expiration');
              }}
              label="Expiration Ratio"
            />
          </SafeAreaView>
          <View style={styles.buttonContainer}>
            <Button
              icon="hand-left"
              mode="contained"
              style={[styles.button, styles.buttonStop]}
              onPress={() => this.handlerButton('stop')}
              labelStyle={styles.buttonLabel}>
              Stop
            </Button>
            <Button
              icon="air-filter"
              mode="contained"
              style={styles.button}
              onPress={() => this.handlerButton('start')}
              labelStyle={styles.buttonLabel}>
              Start
            </Button>
          </View>
        </>
      </ScreenContainer>
    );
  }
}

const styles = StyleSheet.create({
  main: {
    flex: 8,
    justifyContent: 'center',
    alignItems: 'center',
  },
  spaceUp: {
    flex: 1,
  },
  buttonContainer: {
    flex: 4,
    justifyContent: 'space-around',
    alignItems: 'center',
    flexDirection: 'row',
  },
  button: {
    backgroundColor: '#25b35e',
  },
  buttonLabel: {
    fontSize: 25,
  },
  buttonStop: {
    backgroundColor: 'red',
  },
});

SettingsScreen.propTypes = {
  writeNewSettings: PropTypes.func.isRequired,
};

export default SettingsScreen;
