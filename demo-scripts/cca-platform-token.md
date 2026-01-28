### Example: Query platform-token in CVM on OpenCCA
```sh
# in CVM:
taskset -c 0 bash -lc 'name=demo$$; report=/sys/kernel/config/tsm/report/$name; mkdir $report; head -c 64 /dev/urandom > $report/inblob; cat $report/outblob > /tmp/cca-token.cbor; rmdir $report; ls -al /tmp/cca-*'
```
```sh
go install github.com/veraison/evcli/v2@latest
```
```json
b@bmobile:~/bdata/work/opencca/fosdem/opencca$ $HOME/go/bin/evcli cca print --token cca-token.cbor
{
  "cca-platform-token": {
    "cca-platform-profile": "http://arm.com/CCA-SSD/1.0.0",
    "cca-platform-challenge": "tZc8touqn8VVWHhrfsZ/aeQN9bpaqSHNDCf0BYegEeo=",
    "cca-platform-implementation-id": "f0VMRgIBAQAAAAAAAAAAAAMAPgABAAAAUFgAAAAAAAA=",
    "cca-platform-instance-id": "AQcGBQQDAgEADw4NDAsKCQgXFhUUExIREB8eHRwbGhkY",
    "cca-platform-config": "z8/Pzw==",
    "cca-platform-lifecycle": 12291,
    "cca-platform-sw-components": [
      {
        "measurement-type": "RSE_BL1_2",
        "measurement-value": "micfKpFrC27mzsskJvCzIG7wdFeL5V2byU9vP+Orhqo=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "RSE_BL2",
        "measurement-value": "U8I05ehHK2rFHBrhyrP+BvrQU7646/2Jd7AQZVv908M=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "RSE_S",
        "measurement-value": "ESHPzNWRPwpj/sQKb/1E6mT53BNcZmNLoAHRC89DAqI=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "AP_BL1",
        "measurement-value": "FXG17Hi9aFEr94MLtqKkSyBHx99XvOeeuKHA5b6gpQE=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "AP_BL2",
        "measurement-value": "EBWbryYrQ6ktldtZ2uH3LGRRJzAWYeCjzk44spWpfFg=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "SCP_BL1",
        "measurement-value": "EBIuhWs/zUnwY2NjF0dhSctzChqhz6rYGFUrcvVtb2g=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "SCP_BL2",
        "measurement-value": "qmehabC7oheqCqiKZTRpIMhMQkR8NrpffqZfQiwf5dg=",
        "signer-id": "8UtJh5BLy1gU5EWaBX7U0g9YpjMVIoinYSFNzSh4C1Y=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "AP_BL31",
        "measurement-value": "Lm0xpZg6kSUb+uWu+hwKGdi6PPYB0OinBrTPqWYaa4o=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "RMM",
        "measurement-value": "oftQ5shvrhZ57zNRKW/WcTQRoIz43ReQpP0F+uhogWQ=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "HW_CONFIG",
        "measurement-value": "GiUkApcvYFf6U8wXK1K5/8ppjhgxH6zQ87Buyq73nhc=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "FW_CONFIG",
        "measurement-value": "mpKtvAzuOO9ljHHOGxv4xlZo8Wa/shNkTIlcyxrQeiU=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "TB_FW_CONFIG",
        "measurement-value": "I4kDGAzBBOwsXYs/IMW8YbOJ7AqWffjMIIzcfNRUF08=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      },
      {
        "measurement-type": "SOC_FW_CONFIG",
        "measurement-value": "5sIejSYP5xiC3r2zOdJAKiynZIUpvCMD9IZJvOA4ABc=",
        "signer-id": "U3h5YwdTXfPsjYsVouLcVkFBnD0wYM/jIjjA+pc/eqM=",
        "measurement-description": "sha-256"
      }
    ],
    "cca-platform-service-indicator": "https://veraison.example/.well-known/veraison/verification",
    "cca-platform-hash-algo-id": "sha-256"
  },
  "cca-realm-delegated-token": {
    "cca-realm-profile": "tag:arm.com,2023:realm#1.0.0",
    "cca-realm-challenge": "C++BbXEil+wCmWC8Mmkdcq+Rx6XhF9kyX5Nvm3cvrIfaBJRDMlqaRo7B9emwJTwP58UHZ6J2mfPx6s4ren6+Uw==",
    "cca-realm-personalization-value": "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==",
    "cca-realm-initial-measurement": "RlHDwefz5x8xkeBGMTqp324cIkaQ7rgx4IS/PWeVfxY=",
    "cca-realm-extensible-measurements": [
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=",
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=",
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=",
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA="
    ],
    "cca-realm-hash-algo-id": "sha-256",
    "cca-realm-public-key": "pAECIAIhWDB2+YgJG+WF7UGAGuz6uFhUjGMFfhaw5nYSC70NL5wp4FbF1BoBMOucIVF4mdwjFGsiWDAo4bBivT6ksxX9IZ8cu1KMtudMpJvhZ3NzT2GhymEDGyu/PZGPL5T/xCKOUJGVRK4=",
    "cca-realm-public-key-hash-algo-id": "sha-256"
  }
}
```