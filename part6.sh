TOKEN="5455129035:AAFtrar1Y3zM76nxSiAYEHUSxLYpvRtJuoE"
ID="923543834"

if [[ "$CI_JOB_STAGE" == "deploy" ]]      
then
    curl -s --max-time 10 -X POST https://api.telegram.org/bot$TOKEN/sendMessage -d chat_id=$ID -d text="Pipelines ID is $CI_PIPELINE_ID%0ACD is $CI_JOB_STATUS"
elif [[ "$CI_JOB_STAGE" == "test" ]]
then
    curl -s --max-time 10 -X POST https://api.telegram.org/bot$TOKEN/sendMessage -d chat_id=$ID -d text="Pipelines ID is $CI_PIPELINE_ID%0ACI is $CI_JOB_STATUS"
fi

if [[ "$CI_JOB_STAGE" != "test" ]] && [[ "$CI_JOB_STAGE" != "deploy" ]] && [[ "$CI_JOB_STATUS" != "success" ]]
then
    curl -s --max-time 10 -X POST https://api.telegram.org/bot$TOKEN/sendMessage -d chat_id=$ID -d text="Pipelines ID is $CI_PIPELINE_ID%0ACI is $CI_JOB_STATUS"
fi
