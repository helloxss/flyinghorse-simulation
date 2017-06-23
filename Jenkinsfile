#!/usr/bin/env groovy

def builds = [:]
def common        // for storing the groovy script

node { // for loading the shared functions
    git 'git@tainan.tispace.com:tools/shareGroovy.git'
    common = load 'common.groovy'
}

builds['single-node'] = {
    node {
        def workspace = pwd()

        stage('Single-Node Checkout Source Code') {
            common.checkoutSource()
        }

        stage ('Test Single Node') {
            try {
                timeout(10) {  // execute at most 10 minutes
                    sh '''
                        cd exe/single-node
                        ./test.sh
                        python ../../tools/ci_test.py result.csv 1e-5 > test_result
                    '''
                    archiveArtifacts artifacts: 'exe/single-node/result.csv, exe/single-node/test_result',
                                     fingerprint: true
                }
            }
            catch (error) {
                common.notifyResult('FAILURE')
                throw error
            }
        }
    }
}

builds['SIL'] = {
    node {
        def workspace = pwd()

        stage('SIL Checkout Source Code') {
            common.checkoutSource()
        }

        stage ('Test SIL') {
            try {
                timeout(10) {  // execute at most 10 minutes
                    sh '''
                        cd exe/SIL
                        ./test.sh
                        python ../../tools/ci_test.py result.csv 1e-5 > test_result
                    '''
                    archiveArtifacts artifacts: 'exe/SIL/result.csv, exe/SIL/test_result',
                                     fingerprint: true
                }
            }
            catch (error) {
                common.notifyResult('FAILURE')
                throw error
            }
        }
    }
}

builds['PIL'] = {
    node {
        def workspace = pwd()

        stage('PIL Checkout Source Code') {
            common.checkoutSource()
        }

        stage ('Test PIL') {
            try {
                timeout(10) {  // execute at most 10 minutes
                    sh '''
                        cd exe/PIL/slave
                        trick-CP
                        cd ../master
                        ./test.sh
                        python ../../../tools/ci_test.py result.csv 1e-5 > test_result
                    '''
                    archiveArtifacts artifacts: 'exe/PIL/master/result.csv, exe/PIL/master/test_result',
                                     fingerprint: true
                }
            }
            catch (error) {
                common.notifyResult('FAILURE')
                throw error
            }
        }
    }
}

builds['Testing'] = {
    node {
        def workspace = pwd()

        stage('Testing Checkout Source Code') {
            common.checkoutSource()
        }

        stage ('Check Style') {
            try {
                sh '''
                    ./tools/lint.sh junit 2> style_report.xml
                '''
            }
            catch (error) {
                common.notifyResult('FAILURE')
                throw error
            }
            finally {
                junit keepLongStdio: true, testResults: 'style_report.xml'
            }
        }
    }
}

parallel builds

node { // for success slack notification
    common.notifyResult('SUCCESS')
}
